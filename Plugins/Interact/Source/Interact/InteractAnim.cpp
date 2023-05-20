// Copyright Jerónimo Barraco-Mármol

#include "InteractAnim.h"

#include "Interact/CInteract.h"


AInteractAnim::AInteractAnim(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CCurve(TEXT("/Game/LifeDev/Game/Chaps/All/Interact/Door00/C_Door00"));
	AnimCurve = CCurve.Object;
}

void AInteractAnim::BeginPlay() {
	Super::BeginPlay();
	TClosed = IRoot->GetRelativeTransform();
	TClosed.BlendFromIdentityAndAccumulate(TOpen, TAnim, (ScalarOne));
}


void AInteractAnim::Tick(float DT) {
	Super::Tick(DT);
	// to allow to dis-able tick
	if (!IsAnimating) return;

	// adjust for duration
	const float ndt = DT/AnimDuration;
	AnimProgress += ndt;
	if (AnimProgress>=1.0) {
		SetIsAnimating(false);
		return;
	}

	const float Alpha = IsValid(AnimCurve)? AnimCurve->GetFloatValue(AnimProgress) : AnimProgress;
	const float Blend = IsOpen ? Alpha : 1.0 - Alpha;
	// Thanks Tim! this actually works very well!
	FTransform TNew;
	TStart.BlendFromIdentityAndAccumulate(TNew, TAnim, (const ScalarRegister) Blend);
	IRoot->SetRelativeTransform(TNew);
}

void AInteractAnim::SetText_Implementation() {
	Interact->Text = Texts[IsOpen?0:1];
}

void AInteractAnim::SetIsAnimating(bool NewIsAnimating) {
	IsAnimating = NewIsAnimating;
	if (IsAutoTick) SetActorTickEnabled(IsAnimating);
	if (!IsAnimating) return;

	TStart = IsOpen ? TOpen : TClosed;
	AnimProgress = 0.0;
}

void AInteractAnim::Trigger_Implementation() {
	if (IsAnimating) return;
	
	SetText();
	SetIsAnimating(true);
	// change the flag after we start rotating. now the change has happened.
	IsOpen = !IsOpen;
	UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
}
