// Copyright Jerónimo Barraco-Mármol

#include "CInteractor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "JUtils/Net/JUtilsNet.h"

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
	Super::SetAutoActivate(false);
	SetIsReplicated(false); // this one is independent on each client
}

void UCInteractor::Deactivate() {
	Super::Deactivate(); // disables tick. which will disable hover.
	PrimaryComponentTick.SetTickFunctionEnable(false); // Believe it or not it WON'T disable tick without this.
	DoEnd(); // force clearing currently selected
}

void UCInteractor::Activate(const bool Reset) {
	Super::Activate(Reset);
	PrimaryComponentTick.SetTickFunctionEnable(true); // Believe it or not it WON'T disable tick without this.
}

void UCInteractor::SrvTrigger_Implementation(const UCInteract* const Comp) const {
	if (!IsValid(Comp)) return;
	Comp->Trigger();
}

void UCInteractor::TryTrigger() {
	const UCInteract* const PHover = HoverComp.Get();
	if (!IsValid(PHover)) return;

	// decides HERE whether to trigger on the server or client (instead of the CInteract).
	// because the CInteractor is owned by the player controller, hence can call RPCs.
	// Also, the Interact is (should be) owned by the server.
	if (PHover->GetIsReplicated()) {
		SrvTrigger(PHover);
		return;
	}

	PHover->Trigger();
}

bool UCInteractor::TryGrab(const bool IsGrab) {
	UCInteract* const PGrabbed = GrabbedComp.Get();
	UCInteract* const PHover = HoverComp.Get();
	if (IsGrab) {
		if (IsValid(PGrabbed)) {
			UE_LOG(LogCInteractor, Warning, TEXT("Can't grab. i'm already grabbing"));
			return false;
		}
		if (!IsValid(PHover)) {
			UE_LOG(LogCInteractor, Warning, TEXT("Can't grab. nothing to grab."));
			return false;
		}

		// Re-parenting is left to the CInteract
		const bool Ok = PHover->TryGrab(IsGrab, this);
		if (!Ok) {
			UE_LOG(LogCInteractor, Warning, TEXT("Can't grab. CInteract did not want to (probably not grabbable)."));
			return false;
		}

		GrabbedComp = PHover;
		return true;
	}
	/// ungrabbing

	if (!IsValid(PGrabbed)) {
		UE_LOG(LogCInteractor, Warning, TEXT("%hs: Can't ungrab because i have nothing grabbed"), __func__);
		return false;
	}

	UCInteract* const Old = PGrabbed;
	GrabbedComp = nullptr; // not my child anymore :'(

	UPhysicsHandleComponent* const PGrabber = GrabHandler.Get();
	// release of phys components is done here.
	if (PGrabber) {
		PGrabber->ReleaseComponent();
		PGrabber->Deactivate();
	}
	
	// make the Interact do its reparenting and signaling
	Old->TryGrab(false, nullptr);// intentionally ignoring the return value

	return true;
}

EItemUseResult UCInteractor::TryUseItem(const FName Name) const {
	const UCInteract* const PHover = HoverComp.Get();
	// i can't see the inventory from here!
	if (!IsValid(PHover)) {
		UE_LOG(LogCInteractor, Warning, TEXT("%hs Nothing to use the item with"), __func__);
		return EItemUseResult::NO_TARGET;
	}
	
	AActor* const Src = PHover->GetOwner();
	AInteract* const Actor = Cast<AInteract>(Src);
	if (UNLIKELY(!IsValid(Actor))) {
		UE_LOG(LogCInteractor, Warning, TEXT("%hs: The hover actor is not an interact. Can't use the item."),
			__func__);
		return EItemUseResult::NO_TARGET;
	}

	const EItemUseResult& Result = Actor->TryUseItem(Name);
	return Result;
}

void UCInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	// UE_LOG(LogCInteractor, Log, TEXT("%hs: %s. Server=%i, Role=%s."),
	// __func__, *GetNameSafe(this),
	// JU_IsServerSide, *UEnum::GetValueAsString(GetOwnerRole()));

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

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

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
	// Luckily the solution is simple. disable when not needed.
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
	UCInteract* const PHover = HoverComp.Get();
	// not checking for isvalid here in case the obj was destroyed.
	// (Though i'm not certain whether UE will nullify this pointer, in case it will). 
	if (!PHover) return;

	if (IsValid(PHover)) PHover->Hover(false, nullptr);
	OnHover.Broadcast(false, PHover);

	HoverComp = nullptr;
}

void UCInteractor::DoStart(UCInteract* const Component) {
	const UCInteract* const PHover = HoverComp.Get();
	// on every tick almost
	// skip retries
	if (Component == PHover) return;

	UE_LOG(LogCInteractor, Log, TEXT("%hs: %s"),
		__func__, *GetNameSafe(this));

	DoEnd(); // does checks and nullifies

	if (!IsValid(Component)) return;

	HoverComp = Component;
	Component->Hover(true, Cast<APawn>(GetOwner()));
	OnHover.Broadcast(true, Component);
}
