// Copyright Jerónimo Barraco-Mármol

#include "CInteractor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "JUtils/Net/JNetUtils.h"

#include "CInteract.h"
#include "Interact.h"

DEFINE_LOG_CATEGORY_STATIC(LogCInteractor, Log, Log)

#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	// EDrawDebugTrace::Type DrawType = EDrawDebugTrace::None;
	constexpr static EDrawDebugTrace::Type DrawType = EDrawDebugTrace::ForOneFrame;
#else
	constexpr static EDrawDebugTrace::Type DrawType = EDrawDebugTrace::None;
#endif

static ETraceTypeQuery TraceType = TraceTypeQuery1;

UCInteractor::UCInteractor(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(false);
	PrimaryComponentTick.TickInterval = .1f; // 100 ms is enough
	Super::SetAutoActivate(false);
	SetIsReplicated(false); // this one is independent on each client
}

void UCInteractor::Deactivate() {
	Super::Deactivate(); // disables tick. which will disable hover.
	PrimaryComponentTick.SetTickFunctionEnable(false); // Believe it or not it WON'T disable tick without this.
	DoEnd(); // force clearing currently selected
}

void UCInteractor::Activate(bool Reset) {
	Super::Activate(Reset);
	PrimaryComponentTick.SetTickFunctionEnable(true); // Believe it or not it WON'T disable tick without this.
}

void UCInteractor::SrvTrigger_Implementation(UCInteract* Comp) {
	if (!IsValid(Comp)) return;
	Comp->Trigger();
}

void UCInteractor::TryTrigger() {
	if (!IsValid(InterComp)) return;

	// decides HERE whether to trigger on the server or client (instead of the CInteract).
	// because the CInteractor is owned by the player controller, hence can call RPCs.
	// Also, the Interact is (should be) owned by the server.
	if (InterComp->GetIsReplicated()) {
		SrvTrigger(InterComp);
		return;
	}

	InterComp->Trigger();
}

bool UCInteractor::TryGrab(bool IsGrab) {
	if (IsGrab) {
		if (IsValid(GrabbedComp)) {
			UE_LOG(LogCInteractor, Warning, TEXT("Can't grab. i'm already grabbing"));
			return false;
		}
		if (!IsValid(InterComp)) {
			UE_LOG(LogCInteractor, Warning, TEXT("Can't grab. nothing to grab."));
			return false;
		}

		// Re-parenting is left to the CInteract
		const bool Ok = InterComp->TryGrab(IsGrab, this);
		if (!Ok) {
			UE_LOG(LogCInteractor, Warning, TEXT("Can't grab. CInteract did not want to (probably not grabbable)."));
			return false;
		}

		GrabbedComp = InterComp;
		return true;
	}

	if (!IsValid(GrabbedComp)) {
		UE_LOG(LogCInteractor, Warning, TEXT(" Can't ungrab because i have nothing grabbed"));
		return false;
	}

	UCInteract* const Old = GrabbedComp;
	GrabbedComp = nullptr; // not my child anymore :'( (billie jean something ...)

	// release of phys components is done here.
	if (GrabHandler) GrabHandler->ReleaseComponent();
	
	// make the Interact do its reparenting and signaling
	Old->TryGrab(false, nullptr);// intentionally ignoring the return value

	return true;
}

EItemUseResult UCInteractor::TryUseItem(const FName& Name) const {
	// i can't see the inventory from here!
	if (!IsValid(InterComp)) {
		UE_LOG(LogCInteractor, Warning, TEXT("Nothing to use the item with"));
		return EItemUseResult::NO_TARGET;
	}
	
	AActor* const Src = InterComp->GetOwner();
	AInteract* const Actor = Cast<AInteract>(Src);
	if (!IsValid(Actor)) {
		UE_LOG(LogCInteractor, Warning, TEXT("Not a valid actor to use the item with."));
		return EItemUseResult::NO_TARGET;
	}

	const EItemUseResult& Result = Actor->TryUseItem(Name);
	return Result;
}

void UCInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	// UE_LOG(LogCInteractor, Log, TEXT("%hs: %s. Server=%i, Role=%s."),
				// __func__, *GetNameSafe(this),
				// SU_IsServer, *UEnum::GetValueAsString(GetOwnerRole()));

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	const FVector& Start = GetComponentLocation();
	const FVector& Direction = GetComponentRotation().Vector() * TraceLen;
	const FVector& End = Start + Direction;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, false, 1.f);
	// GetWorld()->LineTraceSingleByChannel(Hit, Start, End, InteractChannel);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	
	// need to use this, unfortunately, because this variable doesn't exist otherwise
	#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	Params.bDebugQuery = true;
	#endif

	UWorld* const World = GetWorld();
	if (!World) return;

	if (TraceSize > 1.0) {
		static const TArray<AActor*> ArrEmpty;
		UKismetSystemLibrary::SphereTraceSingle(
			GetOwner(), Start, End,TraceSize,
			TraceType,false, ArrEmpty, DrawType,
			Hit, true
		);
	} else
		World->LineTraceSingleByChannel(Hit, Start, End, InteractChannel, Params);
	
	USceneComponent* const Component = Hit.Component.IsValid() ? Hit.Component.Get() : nullptr;
	UCInteract* const Interact = Cast<UCInteract>(Component);
	DoStart(Interact);
}

void UCInteractor::BeginPlay() {
	Super::BeginPlay();
	TraceType = UEngineTypes::ConvertToTraceType(InteractChannel);

	// replication makes everything more complex.
	// Luckly the solution is simple. disable when not needed.
	// allow it to work in standalone though.
	// this prevents rogue hover and sound effects.
	if (!JU_IsStandalone && GetOwnerRole() != ROLE_AutonomousProxy) {
			Deactivate(); // should disable tick, which is the core of the hover.
		UE_LOG(LogCInteractor, Log, TEXT("%hs: %s: Disabling because it's not autonomous. Server=%i, Role=%s."),
			__func__, *GetNameSafe(this),
			JU_IsServerSide, *UEnum::GetValueAsString(GetOwnerRole()));
	}
}

void UCInteractor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Deactivate();
	Super::EndPlay(EndPlayReason);
}

void UCInteractor::DoEnd() {
	// not checking for isvalid here in case the obj was destroyed.
	// (Though i'm not certain whether UE will nullify this pointer, in case it will). 
	if (!InterComp) return;

	if (IsValid(InterComp)) InterComp->Hover(false);

	OnToggle.Broadcast(false, InterComp);
	OnEnd.Broadcast(InterComp);

	InterComp = nullptr;
}

void UCInteractor::DoStart(UCInteract* Component) {
	// on every tick almost
	// skip retries
	if (Component == InterComp) return;

	UE_LOG(LogCInteractor, Log, TEXT("%hs: %s. Server=%i, Role=%s."),
		__func__, *GetNameSafe(this),
		JU_IsServerSide, *UEnum::GetValueAsString(GetOwnerRole()));

	// notifies on changes
	if (InterComp) {
		DoEnd();
		InterComp = nullptr; // clear after doEnd in case someone needs to access it 
	}

	if (!IsValid(Component)) return;

	InterComp = Component;
	InterComp->Hover(true);

	OnToggle.Broadcast(true, InterComp);
	OnBegin.Broadcast(InterComp);
}
