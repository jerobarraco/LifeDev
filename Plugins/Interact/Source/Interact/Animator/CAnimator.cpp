// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimator.h"

DEFINE_LOG_CATEGORY_STATIC(LogCAnimator, Log, Log)

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
	UE_LOG(LogCAnimator, Log, TEXT("%hs o=%s isReversed=%i isLooping=%i isBouncing=%i"),
		__func__, *GetNameSafe(this), IsReversed, IsLooping, IsBouncing);
	Activate(true);
}

void UCAnimator::TickManual(float DeltaSeconds) {
	if (!IsActive()) return; // very important since this will be spammed

	// basic tick interval for manual ticks
	DTAcum += DeltaSeconds;
	if (DTAcum < GetComponentTickInterval()) return;

	DoTick(DTAcum);
	DTAcum = 0.0;
}

void UCAnimator::Finish() {
	UE_LOG(LogCAnimator, Log, TEXT("%hs o=%s isLooping=%i isBouncing=%i"),
		__func__, *GetNameSafe(this), IsLooping, IsBouncing);
	// check if we can continue at all
	if (!IsLooping && !IsBouncing) {
		Deactivate();
		return;
	}

	End(); // it technically ended (do here since stop will trigger end too)

	/// start the new one
	
	// important to reset the progress.
	// this is ok, since if it's reversed then the end of one == the start of the reversed
	// also if not bouncing we want to start over.
	Progress = 0.0;
	if (IsBouncing) { // reverse the reversed
		Flip(); // IsReversed = !IsReversed;
		// bounce only once if not looping
		if (!IsLooping) IsBouncing = false; 
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
	// get the alpha. if there's a curve use it, else, if there's a code curve use it. otherwise it's the nprog
	const float Alpha =
		IsValid(Curve) ? Curve->GetFloatValue(NProg) :
		(CodeCurve.IsBound() ? CodeCurve.Execute(NProg): NProg);

	if (Debug)
		UE_LOG(LogCAnimator, Log, TEXT("%hs p=%.5f a=%.5f np=%.5f n=%s"),
			__func__, Progress, Alpha, NProg, *GetNameSafe(GetOwner()));

	Update(Alpha); // update child objects

	OnUpdate.Broadcast(Progress, Alpha);

	/// restart if needed
	if (Finished) Finish();
}

void UCAnimator::DeInit() {
	BindTo(nullptr);
}

void UCAnimator::BindTo(UCAnimator* NewParent) {
	UE_LOG(LogCAnimator, Log, TEXT("%hs o=%s parent=%s"),
		__func__, *GetNameSafe(this), *GetNameSafe(NewParent));
	if (IsValid(Parent)) Parent->OnUpdate.RemoveAll(this);
	Parent = nullptr;

	if (!IsValid(NewParent)) return;
	
	Parent = NewParent;
	Parent->OnUpdate.AddUniqueDynamic(this, &UCAnimator::ChildUpdate);
}

void UCAnimator::ChildUpdate(const float T, const float Alpha) {
	Progress = T;
	Update(Alpha);
}

void UCAnimator::Update_Implementation(float Alpha) {
	UE_LOG(LogCAnimator, Verbose, TEXT("%hs o=%s alpha=%3.5f"),
		__func__, *GetNameSafe(GetOwner()), Alpha);
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
}

void UCAnimator::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Deactivate();
	DeInit();
	Super::EndPlay(EndPlayReason);
}

void UCAnimator::TickComponent(float DT, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DT, TickType, ThisTickFunction);
	DoTick(DT);
}

void UCAnimator::Activate(const bool bReset) {
	// activate and deactivate will set/unset tick enabled.
	const bool WasActive = IsActive();
	UE_LOG(LogCAnimator, Log, TEXT("%hs o=%s Reset=%i WasActive=%i"),
		__func__, *GetNameSafe(this), bReset, WasActive);
	
	Super::Activate(bReset);

	if (bReset) {
		Progress = 0;
		DTAcum = 0;
	}

	if (!WasActive || bReset) Begin();
}

void UCAnimator::Deactivate() {
	const bool WasActive = IsActive();
	UE_LOG(LogCAnimator, Log, TEXT("%hs o=%s WasActive=%i"),
		__func__, *GetNameSafe(this), WasActive);
	Super::Deactivate();
	DTAcum = 0;

	// note that progress is not cleared here. that's important for mirroring.
	if (WasActive) End();
}
