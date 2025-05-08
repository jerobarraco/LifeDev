// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "CInteractor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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

void UCInteractor::TryTrigger() {
	const UCInteract* const PHover = HoverComp.Get();
	if (UNLIKELY(!IsValid(PHover))) return;

	PHover->Trigger();
	OnTrigger.Broadcast(PHover);
}

bool UCInteractor::TryGrab(const bool IsGrab) {
	UCInteract* const PGrabbed = GrabbedComp.Get();
	UCInteract* const PHover = HoverComp.Get();
	if (IsGrab) {
		if (UNLIKELY(IsValid(PGrabbed))) {
			UE_LOG(LogCInteractor, Warning, TEXT("%hs: Can't grab. i'm already grabbing."), __func__);
			return false;
		}
		if (UNLIKELY(!IsValid(PHover))) {
			UE_LOG(LogCInteractor, Warning, TEXT("%hs: Can't grab. nothing to grab."), __func__);
			return false;
		}

		// Re-parenting is left to the CInteract
		const bool Ok = PHover->TryGrab(IsGrab, this);
		if (!Ok) {
			UE_LOG(LogCInteractor, Warning, TEXT("%hs: Can't grab. CInteract did not want to (probably not grabbable)."),
				__func__);
			return false;
		}

		GrabbedComp = PHover;
		return true;
	}
	/// ungrabbing

	if (UNLIKELY(!IsValid(PGrabbed))) {
		UE_LOG(LogCInteractor, Warning, TEXT("%hs: Can't ungrab because i have nothing grabbed"), __func__);
		return false;
	}

	UCInteract* const Old = PGrabbed;
	GrabbedComp = nullptr; // not my child anymore :'(

	UPhysicsHandleComponent* const PGrabber = GrabHandler.Get();
	// release of phys components is done here.
	if (LIKELY(PGrabber)) {
		PGrabber->ReleaseComponent();
		PGrabber->Deactivate();
	}
	
	// make the Interact do its reparenting and signaling
	Old->TryGrab(false, nullptr);// intentionally ignoring the return value

	return true;
}

EItemUseResult UCInteractor::TryUseItem(const FName Name) const {
	const UCInteract* const PHover = HoverComp.Get();
	// i can't see the inventory from here! this is the plugin.
	if (UNLIKELY(!IsValid(PHover))) {
		UE_LOG(LogCInteractor, Warning, TEXT("%hs Nothing to use the item with."), __func__);
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
}

void UCInteractor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Deactivate();
	Super::EndPlay(EndPlayReason);
}

void UCInteractor::DoEnd() {
	UCInteract* const PHover = HoverComp.Get();
	HoverComp = nullptr; // important to nullify.
	// if not valid just return. it won't be nice to broadcast onHover with an invalid component.
	if (UNLIKELY(!IsValid(PHover))) return;

	PHover->Hover(false, nullptr);
	OnHover.Broadcast(false, PHover);
}

void UCInteractor::DoStart(UCInteract* const Component) {
	const UCInteract* const PHover = HoverComp.Get();
	const bool ValidHover = IsValid(PHover);
	// TODO fix
	// on every tick almost
	// skip retries
	// pretty likely it's the same from the previous frame 
	// Valid is important since an object could be destroyed on reward. comp == pHover but it won't be valid anymore.
	if (LIKELY((Component == PHover) & ValidHover)) return;

	UE_LOG(LogCInteractor, Log, TEXT("%hs: %s"),
		__func__, *GetNameSafe(this));

	DoEnd(); // does checks and nullifies. this would allow to clean by calling doStart with null

	if (UNLIKELY(!IsValid(Component) & !ValidHover)) return;

	HoverComp = Component;
	Component->Hover(true, Cast<APawn>(GetOwner()));
	OnHover.Broadcast(true, Component);
}
