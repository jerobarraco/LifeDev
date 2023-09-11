// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "CAnimator.h"

UCAnimator::UCAnimator():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// if this is true, it will start active anyway.
	PrimaryComponentTick.SetTickFunctionEnable(false);
	SetComponentTickInterval(IntervalDefault);
	Super::SetAutoActivate(false);
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Interact/Interact_C.Interact_C"));
	Curve = CCurve.Succeeded() ? CCurve.Object : nullptr;
}

void UCAnimator::PlaySet(bool Reversed, bool Loop, bool Bounce) {
	IsReversed = Reversed;
	IsLooping = Loop;
	IsBouncing = Bounce;
	Play();
}

void UCAnimator::TickManual(float DeltaSeconds) {
	// basic tick interval for manual ticks
	DTAcum += DeltaSeconds;
	// UE_LOG(LogTemp, Log, TEXT("TickManual DTAcum=%3.3f DT=%3.3f"), DTAcum, DeltaSeconds);
	if (DTAcum < GetComponentTickInterval()) return;

	DoTick(DTAcum);
	DTAcum = 0.0;
}

void UCAnimator::Finish() {
	// check if we can continue at all
	if (!IsLooping && !IsBouncing) {
		Stop();
		return;
	}

	End(); // it technically ended (do here since stop will trigger end too)

	/// start the new one
		
	// important to reset the progress.
	// this is ok, since if it's reversed then the end of one == the start of the reversed
	// also if not bouncing we want to start over.
	Progress = 0.0;
	if (IsBouncing) { // reverse the reversed
		IsReversed = !IsReversed;
		// bounce only once if not looping
		if (!IsLooping) {
			IsBouncing = false; 
		}
	}
	Begin(); // it technically started
}

void UCAnimator::DoTick(float DT) {
	// adjust for duration
	const float ndt = DT/Duration;
	Progress += ndt;
	// check for finish before but allow to process
	// that way we ensure we always trigger Progress =1.0 so animations finish where they need to
	const bool Finished = Progress >= 1.0;
	if (Finished) Progress = 1.0; // manual clamp important

	/// process
	// small trick to ensure we can reverse an animation.
	const float NProg = IsReversed ? 1.0 - Progress : Progress;
	const float Alpha =
		IsValid(Curve) ? Curve->GetFloatValue(NProg) :
		(CodeCurve.IsBound() ? CodeCurve.Execute(NProg): NProg);

	if (Debug) {
		UE_LOG(LogTemp, Log, TEXT("AnimTick %05f %05f %05f"), Progress, Alpha, NProg);
	}

	// update child objects
	Update(Alpha);

	// Trigger delegate
	OnUpdate.Broadcast(Progress, Alpha);

	/// restart if needed
	if (Finished) {
		Finish();
	}
}

void UCAnimator::DeInit() {
	BindTo(nullptr);
}

void UCAnimator::BindTo(UCAnimator* NewParent) {
	if (IsValid(Parent)) {
		Parent->OnUpdate.RemoveAll(this);
	}
	Parent = nullptr;

	if (!IsValid(NewParent)) return;
	
	Parent = NewParent;
	Parent->OnUpdate.AddUniqueDynamic(this, &UCAnimator::ChildUpdate);
}

void UCAnimator::ChildUpdate(float T, float Alpha) {
	Progress = T;
	Update(Alpha);
}

void UCAnimator::End_Implementation() {
	OnEnd.Broadcast();
}

void UCAnimator::Begin_Implementation() {
	OnBegin.Broadcast();
}

void UCAnimator::BeginPlay() {
	Super::BeginPlay();
	BindTo(Parent); // rebind to the parent if set
	Deactivate();
}

void UCAnimator::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Stop();
	DeInit();
	Super::EndPlay(EndPlayReason);
}

void UCAnimator::TickComponent(float DT, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DT, TickType, ThisTickFunction);
	DoTick(DT);
}

void UCAnimator::Activate(bool bReset) {
	const bool WasActive = IsActive();
	Super::Activate(bReset);
	if (bReset) {
		Progress = 0;
	}
	if (!WasActive) {
		Begin();
	}
}

void UCAnimator::Deactivate() {
	const bool WasActive = IsActive();
	Super::Deactivate();
	if (WasActive) {
		End();
	}
}
