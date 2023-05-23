// Copyright Jerónimo Barraco-Mármol

#include "CAnimator.h"

UCAnimator::UCAnimator():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CCurve(TEXT("/Interact/C_Interact.C_Interact"));
	Curve = CCurve.Object;
}

void UCAnimator::Play(bool NewIsReversed) {
	IsReversed = NewIsReversed;
	SetIsAnimating(true);
}

void UCAnimator::BeginPlay() {
	Super::BeginPlay();
	// PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.Target = this;
	// PrimaryComponentTick.SetTickFunctionEnable(true);
	// RegisterComponentTickFunctions(true); will crash
	if (IsValid(AnimRoot)) {
		TStart = AnimRoot->GetRelativeTransform();
	}
}

void UCAnimator::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	AnimRoot = nullptr;
}

void UCAnimator::TickComponent(float DT, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DT, TickType, ThisTickFunction);
	// to allow to dis-able tick
	if (!IsAnimating || !IsValid(AnimRoot)) {
		SetIsAnimating(false); return;
	}

	// adjust for duration
	const float ndt = DT/Duration;
	Progress += ndt;
	if (Progress>=1.0) {
		SetIsAnimating(false);
		return;
	}

	const float NProg = IsValid(Curve)? Curve->GetFloatValue(Progress) : Progress;
	// small trick to ensure we can reverse an animation.
	const float Alpha = IsReversed ? 1.0 - NProg : NProg;
	FTransform TNew = TStart;
	if (IsAdditive) {
		TStart.BlendFromIdentityAndAccumulate(TNew, TEnd, (const ScalarRegister) Alpha);
		// TNew.Accumulate(TEnd, (const ScalarRegister) Alpha); // not what i want, does something different with the scale.
	}else {
		// Thanks Tim! this actually works very well!
		TNew.BlendWith(TEnd, Alpha);
	}
	AnimRoot->SetRelativeTransform(TNew);
	
	// UE_LOG(LogTemp, Log, TEXT("Tick  %05f %05f"), Progress, Alpha);
	OnChange.Broadcast(Progress, Alpha);
}

void UCAnimator::SetIsAnimating(bool NewIsAnimating) {
	IsAnimating = NewIsAnimating;
	SetComponentTickEnabled(IsAnimating);
	if (!IsAnimating) {
		OnEnd.Broadcast();
		return;
	}

	Progress = 0.0;
}
