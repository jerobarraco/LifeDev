// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "CAnimator.h"

#include "AnimTracks.h"
// https://doc.qt.io/qt-6/qeasingcurve.html

UCAnimator::UCAnimator():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Interact/Interact_C.Interact_C"));
	Curve = CCurve.Succeeded() ? CCurve.Object : nullptr;
	SetComponentTickInterval(IntervalDefault);
}

void UCAnimator::PlaySet(bool Reversed, bool Loop, bool Bounce) {
	IsReversed = Reversed;
	IsLooping = Loop;
	IsBouncing = Bounce;
	Play();
}

void UCAnimator::DoTick(float DT) {
	if (!IsAnimating) return;
	
	// adjust for duration
	const float ndt = DT/Duration;
	Progress += ndt;
	if (Progress >= 1.0) {
		if (!IsLooping && !IsBouncing) {
			Stop();
			return;
		}
		End(); // it technically ended
		
		// important to reset the progress.
		// this is ok, since if it's reversed then the end of one == the start of the reversed
		// also if not bouncing we want to start over.
		Progress = 0.0;
		if (IsBouncing) { // reverse the reversed
			IsReversed = !IsReversed;
			// only bounce once if not looping
			if (!IsLooping) {
				IsBouncing = false; 
			}
		}
		Begin(); // it technically started
	}

	// small trick to ensure we can reverse an animation.
	const float NProg = IsReversed ? 1.0 - Progress : Progress;
	const float Alpha =
		IsValid(Curve) ? Curve->GetFloatValue(NProg) :
		(CodeCurve.IsBound() ? CodeCurve.Execute(NProg): NProg);

	for (UAnimTrackBase* T: Tracks) {
		T->Update(Alpha);
	}
	Update(Alpha);
	
	// UE_LOG(LogTemp, Log, TEXT("AnimTick %05f %05f %05f"), Progress, Alpha, NProg);
	OnUpdate.Broadcast(Progress, Alpha);
}

void UCAnimator::DeInit() {}

void UCAnimator::AddTrackMatF(UMaterialInstanceDynamic* M, const FName& Name, float FStart, float FEnd) {
	UAnimTrackMatF* T = NewObject<UAnimTrackMatF>();
	T->Mat = M;
	T->Name = Name;
	T->Start = FStart;
	T->End = FEnd;
	Tracks.Add(T);
}

void UCAnimator::End_Implementation() {
	OnEnd.Broadcast();
}

void UCAnimator::Begin_Implementation() {
	OnBegin.Broadcast();
}

void UCAnimator::BeginPlay() {
	Super::BeginPlay();
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

void UCAnimator::SetIsAnimating(bool NewIsAnimating) {
	const bool WasAnimating = IsAnimating;
	Progress = 0.0; // force it because of the if below which can cause new calls
	IsAnimating = NewIsAnimating;

	// reset ticks
	if (IsAnimating) {
		Activate();
	} else {
		Deactivate();
	}

	if (IsAnimating) {
		Begin();
	} else if (WasAnimating) {
		End();
	}
}
