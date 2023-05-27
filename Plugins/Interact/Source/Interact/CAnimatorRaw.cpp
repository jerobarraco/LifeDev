// Copyright Jerónimo Barraco-Mármol

#include "CAnimatorRaw.h"

UCAnimatorRaw::UCAnimatorRaw():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CCurve(TEXT("/Interact/C_Interact.C_Interact"));
	Curve = CCurve.Object;
}

void UCAnimatorRaw::Play(bool NewIsReversed) {
	IsReversed = NewIsReversed;
	SetIsAnimating(true);
}

void UCAnimatorRaw::Stop() {
	SetIsAnimating(false);
}

void UCAnimatorRaw::Update_Implementation(float Alpha) {}

void UCAnimatorRaw::BeginPlay() {
	Super::BeginPlay();
	// PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.Target = this;
	// PrimaryComponentTick.SetTickFunctionEnable(true);
	// RegisterComponentTickFunctions(true); will crash
}

void UCAnimatorRaw::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	SetIsAnimating(false);
	Super::EndPlay(EndPlayReason);
}

void UCAnimatorRaw::TickComponent(float DT, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DT, TickType, ThisTickFunction);
	// to allow to dis-able tick
	if (!IsAnimating) {
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

	Update_Implementation(Alpha);
	// UE_LOG(LogTemp, Log, TEXT("Tick  %05f %05f"), Progress, Alpha);
	OnChange.Broadcast(Progress, Alpha);
}

void UCAnimatorRaw::SetIsAnimating(bool NewIsAnimating) {
	IsAnimating = NewIsAnimating;
	SetComponentTickEnabled(IsAnimating);
	if (!IsAnimating) {
		OnEnd.Broadcast();
		return;
	}

	Progress = 0.0;
}
