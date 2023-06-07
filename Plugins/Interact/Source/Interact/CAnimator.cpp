// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "CAnimator.h"

UCAnimator::UCAnimator():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CCurve(TEXT("/Interact/C_Interact.C_Interact"));
	Curve = CCurve.Object;
}

void UCAnimator::Play(bool Reversed, bool Loop, bool Bounce) {
	IsReversed = Reversed;
	IsLooping = Loop;
	IsBouncing = Bounce;
	SetIsAnimating(true);
}

void UCAnimator::Stop() {
	SetIsAnimating(false);
}

void UCAnimator::End_Implementation() {
	OnEnd.Broadcast();
}

void UCAnimator::Begin_Implementation() {
	OnBegin.Broadcast();
}

void UCAnimator::BeginPlay() {
	Super::BeginPlay();
	// PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.Target = this;
	// PrimaryComponentTick.SetTickFunctionEnable(true);
	// RegisterComponentTickFunctions(true); will crash
}

void UCAnimator::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Stop();
	Super::EndPlay(EndPlayReason);
}

void UCAnimator::TickComponent(float DT, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DT, TickType, ThisTickFunction);
	// to allow to dis-able tick
	if (!IsAnimating) {
		Stop();
		return;
	}

	// adjust for duration
	const float ndt = DT/Duration;
	Progress += ndt;
	if (Progress >= 1.0) {
		if (!IsLooping) {
			Stop();
			return;
		}
		End(); // it technically ended

		if (IsBouncing) {
			IsReversed = !IsReversed;
		}
		Begin(); // it technically started
	}

	const float NProg = IsValid(Curve)? Curve->GetFloatValue(Progress) : Progress;
	// small trick to ensure we can reverse an animation.
	const float Alpha = IsReversed ? 1.0 - NProg : NProg;

	Update_Implementation(Alpha);
	// UE_LOG(LogTemp, Log, TEXT("Tick  %05f %05f"), Progress, Alpha);
	OnUpdate.Broadcast(Progress, Alpha);
}

void UCAnimator::SetIsAnimating(bool NewIsAnimating) {
	const bool WasAnimating = IsAnimating;
	IsAnimating = NewIsAnimating;
	SetComponentTickEnabled(IsAnimating);
	Progress = 0.0; // force it because of the if below

	if (IsAnimating) {
		Begin();
	} else if (WasAnimating) {
		End();
	}
}
