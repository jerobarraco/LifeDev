// Copyright Jerónimo Barraco-Mármol

#include "InteractAnim.h"

#include "CAnimator.h"
#include "Interact/CInteract.h"


AInteractAnim::AInteractAnim():Super() {
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	Animator = CreateDefaultSubobject<UCAnimator>(TEXT("Animator"));
	Animator->AnimRoot = IRoot;
	Animator->RegisterComponent();
}

void AInteractAnim::SetText_Implementation() {
	Interact->Text = Texts[IsOpen?0:1];
}

void AInteractAnim::Trigger_Implementation() {
	if (Animator->GetIsAnimating()) return;

	SetText();
	Animator->Play(IsOpen);

	// change the flag after we start rotating. now the change has happened.
	IsOpen = !IsOpen;
	UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
}