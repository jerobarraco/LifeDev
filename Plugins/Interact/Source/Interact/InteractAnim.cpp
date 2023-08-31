// Copyright Jerónimo Barraco-Mármol

#include "InteractAnim.h"

#include "Interact/CInteract.h"
#include "Animator/CAnimatorMix.h"
#include "Components/AudioComponent.h"
#include "JUtils/CQuickMesh.h"

AInteractAnim::AInteractAnim():Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Super::SetActorTickEnabled(false);
	
	Anim = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimatorMix"));
	Anim->TRoot = IRoot;
	// can't do this, the order of constructors fails. Anim->Mat = Mesh->GetMaterial(0);
}

void AInteractAnim::BeginPlay() {
	Super::BeginPlay();
	Anim->OnBegin.AddUniqueDynamic(this, &AInteractAnim::AnimBegin);
	Anim->OnEnd.AddUniqueDynamic(this, &AInteractAnim::AnimEnd);
}

void AInteractAnim::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Anim->OnBegin.RemoveAll(this);
	Anim->OnEnd.RemoveAll(this);
}

void AInteractAnim::SetText_Implementation() {
	Super::SetText_Implementation();
	if (Texts.Num()<1) {
		UE_LOG(LogTemp, Warning, TEXT("AInteractAnim.SetText: Object has no text to set"));
		return;
	}
	if (Texts.Num()<2) {
		Interact->Text = Texts[0];
		return;
	}
	Interact->Text = Texts[IsOpen?1:0];
}

void AInteractAnim::Trigger_Implementation() {
	// set the flag before so that the sound triggers are consistent
	IsOpen = !IsOpen;
	if (AnimEnabled) {
		// this creates so many issues. notice how it's set.
		Anim->PlaySet(!IsOpen);
		UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
	}
	Super::Trigger_Implementation(); // trigger the trigger sound and calls set text
	// SetText(); // change the text after the flag has changed
}

bool AInteractAnim::TryTrigger_Implementation() {
	if (AnimEnabled && Anim->GetIsAnimating()) return false;
	return Super::TryTrigger_Implementation();
}

void AInteractAnim::AnimBegin_Implementation() {
	// at this point the isOpen flag is toggled
	USoundBase* const Snd = IsOpen ? SFX_Open : SFX_Close;
	PlaySFX(Snd);
}

void AInteractAnim::AnimEnd_Implementation() {
	// at this point the isOpen flag is toggled
	USoundBase* const Snd = IsOpen ? SFX_OpenEnd : SFX_CloseEnd;
	PlaySFX(Snd);
}

void AInteractAnim::SetMobility(EComponentMobility::Type Mobility) {
	Mesh->SetMobility(Mobility);
	Interact->SetMobility(Mobility);
	RootComponent->SetMobility(Mobility);
	IRoot->SetMobility(Mobility);
	SFX->SetMobility(Mobility);
	AnimEnabled = Mobility == EComponentMobility::Movable;
}
