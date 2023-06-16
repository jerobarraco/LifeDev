// Copyright Jerónimo Barraco-Mármol

#include "InteractAnim.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "CAnimatorMix.h"

AInteractAnim::AInteractAnim():Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false);
	
	Anim = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimatorMix"));
	Anim->TRoot = IRoot;
	Anim->Mat = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(0));

	SFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX"));
	SFX->SetupAttachment(IRoot);
	SFX->SetAutoActivate(false);
	SFX->SetHiddenInGame(true);
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
	if (Texts.Num()<2) {
		if (Texts.Num()>0) {
			Interact->Text = Texts[0];
		}
		return;
	}
	Interact->Text = Texts[IsOpen?1:0];
}

void AInteractAnim::Trigger_Implementation() {
	Super::Trigger_Implementation();

	if (!AnimEnabled) return;
	// set the flag before so that the sound triggers are consistent
	IsOpen = !IsOpen;
	SetText(); // change the text after the flag has changed
	// this creates so many issues. notice how it's set.
	Anim->Play(!IsOpen);

	UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
}

void AInteractAnim::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	PlaySFX(SFX_Locked);
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

void AInteractAnim::PlaySFX(USoundBase* Snd) {
	if (!IsValid(Snd)) return;
	SFX->SetHiddenInGame(false);
	SFX->SetSound(Snd);
	SFX->SetActive(true, true);
	SFX->Play(0);
}
