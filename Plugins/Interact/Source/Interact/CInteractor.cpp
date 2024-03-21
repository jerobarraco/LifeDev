// Copyright Jerónimo Barraco-Mármol

#include "CInteractor.h"

#include "Components/ArrowComponent.h"

#include "CInteract.h"
#include "Interact.h"
#include "InteractTypes.h"
#include "Kismet/KismetSystemLibrary.h"

#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	// EDrawDebugTrace::Type DrawType = EDrawDebugTrace::None;
	EDrawDebugTrace::Type DrawType = EDrawDebugTrace::ForOneFrame;
#else
	EDrawDebugTrace::Type DrawType = EDrawDebugTrace::None;
#endif

static ETraceTypeQuery TraceType = TraceTypeQuery1;

UCInteractor::UCInteractor(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);
	PrimaryComponentTick.TickInterval = .1f; // 100 ms is enough
	// the arrow doesn't parent correctly. so.. beat it
}

void UCInteractor::SetEnabled(bool Enabled) {
	SetComponentTickEnabled(Enabled);
	DoEnd(); // force clearing currently selected
}

void UCInteractor::TryTrigger() {
	if (!IsValid(InterComp)) return;
	InterComp->Trigger();
}

EItemUseResult UCInteractor::TryUseItem(const FName& Name) const {
	// i can't see the inventory from here!
	if (!IsValid(InterComp)) {
		UE_LOG(LogTemp, Warning, TEXT("Nothing to use the item with"));
		return EItemUseResult::NO_TARGET;
	}
	
	AActor* const Src = InterComp->GetOwner();
	AInteract* const Actor = Cast<AInteract>(Src);
	if (!IsValid(Actor)) {
		UE_LOG(LogTemp, Warning, TEXT("Not a valid actor to use the item with."));
		return EItemUseResult::NO_TARGET;
	}

	const EItemUseResult& Result = Actor->TryUseItem(Name);
	return Result;
}

void UCInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
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
	} else {
		World->LineTraceSingleByChannel(Hit, Start, End, InteractChannel, Params);
	}
	
	USceneComponent* const Component = Hit.Component.IsValid() ? Hit.Component.Get() : nullptr;
	UCInteract* const Interact = Cast<UCInteract>(Component);
	DoStart(Interact);
}

void UCInteractor::BeginPlay() {
	Super::BeginPlay();
	TraceType = UEngineTypes::ConvertToTraceType(InteractChannel);
}

void UCInteractor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DoEnd();
	Super::EndPlay(EndPlayReason);
}

void UCInteractor::DoEnd() {
	if (!InterComp) return;
	if (IsValid(InterComp)) {
		InterComp->Hover(false);
	}
	OnToggle.Broadcast(false, InterComp);
	OnEnd.Broadcast(InterComp);

	InterComp = nullptr;
}

void UCInteractor::DoStart(UCInteract* Component) {
	// skip retries
	if (Component == InterComp) return;

	// notifies on changes
	if (InterComp) {
		DoEnd();
		InterComp = nullptr; // clear after doend in case someone needs to access it 
	}

	if (!IsValid(Component)) return;

	InterComp = Component;
	InterComp->Hover(true);

	OnToggle.Broadcast(true, InterComp);
	OnBegin.Broadcast(InterComp);
}
