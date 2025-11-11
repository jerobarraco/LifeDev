// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "CInteractor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "CInteract.h"

DEFINE_LOG_CATEGORY_STATIC(LogCInteractor, Log, Log)

#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	// EDrawDebugTrace::Type DrawType = EDrawDebugTrace::None;
	constexpr static EDrawDebugTrace::Type DrawType = EDrawDebugTrace::ForOneFrame;
#else
	constexpr static EDrawDebugTrace::Type DrawType = EDrawDebugTrace::None;
#endif

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

void UCInteractor::Activate(const bool Reset) {
	Super::Activate(Reset);
	PrimaryComponentTick.SetTickFunctionEnable(true); // Believe it or not it WON'T disable tick without this.
}

void UCInteractor::SetTrace(const ETraceTypeQuery Trace) {
	TraceType = Trace;
}

void UCInteractor::SetChannel(const ECollisionChannel Chan) {
	Channel = Chan;
	SetTrace(UEngineTypes::ConvertToTraceType(Chan));
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

void UCInteractor::TickComponent(const float DeltaTime, const ELevelTick TickType,
	FActorComponentTickFunction* const ThisTickFunction) {
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
		World->LineTraceSingleByChannel(Hit, Start, End, Channel, Params);
	
	// USceneComponent* const Component = Hit.Component.IsValid() ? Hit.Component.Get() : nullptr;
	USceneComponent* const Component = Hit.Component.Get(); // todo test if this doesn't crash
	UCInteract* const Interact = Cast<UCInteract>(Component);
	DoStart(Interact);
}

void UCInteractor::BeginPlay() {
	Super::BeginPlay();
	// in case the default was changed
	SetTrace(UEngineTypes::ConvertToTraceType(Channel));
}

void UCInteractor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Deactivate();
	Super::EndPlay(EndPlayReason);
}

void UCInteractor::DoEnd() {
	UCInteract* const PHover = HoverComp.Get(); // could be null if it was destroyed. warning.
	HoverComp = nullptr; // important to nullify.

	if (UNLIKELY(!IsHovering)) return; // avoid triggering a delegate when not needed.
	
	IsHovering = false;
	if (LIKELY(IsValid(PHover)))
		PHover->Hover(false, nullptr);

	OnHover.Broadcast(false, PHover); // could be null
}

void UCInteractor::DoStart(UCInteract* const Component) {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const UCInteract* const PHover = HoverComp.Get();
	// can't be up. since ue will nullify the phover on destroy. hence this will be true.
	// but on top would prevent the doEnd
	if (LIKELY(Component == PHover)) return;

	const bool ValidOld = IsValid(PHover);
	const bool ValidNew = IsValid(Component);
	const bool Died = IsHovering & !ValidOld; // was hovering but we've lost track of it
	const bool UnHover = ValidOld & !ValidNew; // have one, but not anymore
	// i could micro-optimize this a bit more, but it will become harder to read.
	if (UNLIKELY(Died | UnHover)) {
		UE_LOG(LogCInteractor, Log, TEXT("%hs: No longer hovering %s"), __func__, *GetNameSafe(this));
		DoEnd();
		return;
	}

	// important when hovering one interact after the other immediately.
	// end the previous. notice no return.
	if (UNLIKELY(ValidOld)) DoEnd();
	if (!ValidNew) return; // safeguard, we could get a doStart with an empty comp

	UE_LOG(LogCInteractor, Log, TEXT("%hs: %s"),
		__func__, *GetNameSafe(this));

	IsHovering = true;
	HoverComp = Component;
	Component->Hover(true, Cast<APawn>(GetOwner()));
	OnHover.Broadcast(true, Component);

	/// look
	// will hide the prompt on invalid. which is a nice side effect.
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimer(LookHandle, this, &UCInteractor::Look, LookTime);
}

void UCInteractor::Look() const {
	const UCInteract* const Comp = HoverComp.Get();
	if(UNLIKELY(!IsValid(Comp))) return;

	// Comp->Look(Cast<APawn>(GetOwner())); // todo
	
	OnLook.Broadcast(Comp);
}
