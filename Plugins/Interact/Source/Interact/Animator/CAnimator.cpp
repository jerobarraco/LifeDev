// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

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

void UCAnimator::PlaySet(const bool Reversed, const bool Loop, const bool Bounce) {
	IsReversed = Reversed;
	IsLooping = Loop;
	IsBouncing = Bounce;
	UE_CLOG(UseLog, LogCAnimator, Log,
		TEXT("%hs o=%s isReversed=%i isLooping=%i isBouncing=%i"),
		__func__, *GetNameSafe(GetOwner()), IsReversed, IsLooping, IsBouncing);
	Activate(true);
}

void UCAnimator::TickManual(const float DeltaSeconds) {
	if (!IsActive()) return; // very important since this will be spammed

	// basic tick interval for manual ticks
	DTAcum += DeltaSeconds;
	if (DTAcum < GetComponentTickInterval()) return;

	DoTick(DTAcum);
	DTAcum = 0.0;
}

void UCAnimator::Finish() {
	UE_CLOG(UseLog, LogCAnimator, Log,
		TEXT("%hs o=%s isReversed=%i isLooping=%i isBouncing=%i"),
		__func__, *GetNameSafe(GetOwner()), IsReversed, IsLooping, IsBouncing);
	const bool CanBounce = IsBouncing && !HasBounced;
	// check if we can continue at all
	if (!IsLooping & !CanBounce) {
		Deactivate();
		return;
	}

	/// loop
	End(); // it technically ended (do here since stop will trigger end too)

	/// start the new one
	
	// important to reset the progress.
	// this is ok, since if it's reversed then the end of one == the start of the reversed
	// also if not bouncing we want to start over.
	Progress = 0.0;
	if (CanBounce) { // reverse the reversed
		Flip(); // IsReversed = !IsReversed;
		// bounce only once if not looping
		// if (!IsLooping) IsBouncing = false;
		if (!IsLooping) HasBounced = true;
	}
	// if !CanBounce implies Looping here
	Begin(); // it technically started
}

void UCAnimator::DoTick(float DT) {
	if (!UseDilation) {
		const UWorld* const World = GetWorld();
		if (UNLIKELY(!World)) return;

		const AWorldSettings* const Settings = World->GetWorldSettings(false, false);
		if (UNLIKELY(!Settings)) return;
		DT /= FMath::Max(UE_SMALL_NUMBER, Settings->TimeDilation); // avoid crash
	}

	// support duration of 0
	if (UNLIKELY(FMath::IsNearlyZero(Duration))) {
		Progress = 1.0;
	} else {
		// adjust for duration
		const float NDT = DT/Duration;
		Progress += NDT;
	}

	// check for finish before but allow to process
	// that way we ensure we always trigger Progress =1.0 so animations finish where they need to
	const bool Finished = Progress >= 1.0;
	if (UNLIKELY(Finished)) Progress = 1.0; // manual clamp important

	/// process
	// small trick to ensure we can reverse an animation.
	const float NProg = IsReversed ? 1.0 - Progress : Progress;
	// get the alpha. if there's a curve use it, else, if there's a code curve use it. otherwise it's the nprog
	const float Alpha =
		IsValid(Curve) ? Curve->GetFloatValue(NProg) :
		(CodeCurve.IsBound() ? CodeCurve.Execute(NProg): NProg);

	UE_CLOG(Debug, LogCAnimator, Log, TEXT("%hs p=%.5f a=%.5f np=%.5f n=%s"),
		__func__, Progress, Alpha, NProg, *GetNameSafe(GetOwner()));

	Update(Alpha); // update child objects

	// call from outside so it's triggered once all the overwritten functions are finished.
	// has the nice side effect that i can call update on constructor.
	OnUpdate.Broadcast(Progress, Alpha);

	/// restart if needed
	if (UNLIKELY(Finished)) Finish();
}

void UCAnimator::DeInit() {
	BindTo(nullptr);
}

void UCAnimator::BindTo(UCAnimator* const NewParent) {
	UE_CLOG(UseLog, LogCAnimator, Log, TEXT("%hs o=%s parent=%s"),
		__func__, *GetNameSafe(GetOwner()), *GetNameSafe(NewParent));

	if (IsValid(Parent)) Parent->OnUpdate.RemoveAll(this);
	Parent = nullptr;

	if (UNLIKELY(!IsValid(NewParent))) return;
	
	Parent = NewParent;
	Parent->OnUpdate.AddUniqueDynamic(this, &UCAnimator::ChildUpdate);
}

void UCAnimator::ChildUpdate(const float T, const float Alpha) {
	Progress = T;
	Update(Alpha);
}

void UCAnimator::Update_Implementation(const float Alpha) {
	UE_CLOG(Debug, LogCAnimator, Verbose, TEXT("%hs o=%s alpha=%3.5f"),
		__func__, *GetNameSafe(GetOwner()), Alpha);
}

void UCAnimator::End_Implementation() {
	UE_CLOG(Debug, LogCAnimator, Verbose, TEXT("%hs o=%s"),
		__func__, *GetNameSafe(GetOwner()));
	OnEnd.Broadcast();
}

void UCAnimator::Begin_Implementation() {
	UE_CLOG(Debug, LogCAnimator, Verbose, TEXT("%hs o=%s"),
		__func__, *GetNameSafe(GetOwner()));

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

void UCAnimator::TickComponent(const float DT, const ELevelTick TickType,
FActorComponentTickFunction* const ThisTickFunction) {
	Super::TickComponent(DT, TickType, ThisTickFunction);
	DoTick(DT);
}

void UCAnimator::Activate(const bool bReset) {
	// activate and deactivate will set/unset tick enabled.
	const bool WasActive = IsActive();
	
	UE_CLOG(UseLog, LogCAnimator, Log,
		TEXT("%hs o=%s Reset=%i WasActive=%i isReversed=%i isLooping=%i isBouncing=%i"),
		__func__, *GetNameSafe(GetOwner()), bReset, WasActive,
		IsReversed, IsLooping, IsBouncing);
	
	Super::Activate(bReset);

	const bool DoRestart = !WasActive | bReset;
	if (!DoRestart) return;
	
	// re-init variables
	Progress = 0;
	DTAcum = 0;
	HasBounced = false;

	Begin();
}

void UCAnimator::Deactivate() {
	const bool WasActive = IsActive();
	UE_CLOG(UseLog, LogCAnimator, Log,
		TEXT("%hs o=%s WasActive=%i isReversed=%i isLooping=%i isBouncing=%i"),
		__func__, *GetNameSafe(GetOwner()), WasActive,
		IsReversed, IsLooping, IsBouncing);
	Super::Deactivate();
	DTAcum = 0;

	// note that progress is not cleared here. that's important for mirroring.
	if (WasActive) End();
}
