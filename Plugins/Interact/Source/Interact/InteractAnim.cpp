// Copyright Jerónimo Barraco-Mármol

#include "InteractAnim.h"

#include "Components/AudioComponent.h"

#include "CAnimatorTrans.h"
#include "Interact/CInteract.h"

AInteractAnim::AInteractAnim():Super() {
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	Animator = CreateDefaultSubobject<UCAnimatorTrans>(TEXT("Animator"));
	Animator->AnimRoot = IRoot;
	
	SFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX"));
	SFX->SetupAttachment(IRoot);
	SFX->SetAutoActivate(false);
	SFX->SetHiddenInGame(true);

	// Animator->RegisterComponent();
}

void AInteractAnim::BeginPlay() {
	Super::BeginPlay();
	Animator->OnBegin.AddUniqueDynamic(this, &AInteractAnim::AnimBegin);
	Animator->OnEnd.AddUniqueDynamic(this, &AInteractAnim::AnimEnd);
}

void AInteractAnim::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Animator->OnBegin.RemoveAll(this);
	Animator->OnEnd.RemoveAll(this);
}

void AInteractAnim::SetText_Implementation() {
	Super::SetText_Implementation();
	Interact->Text = Texts[IsOpen?1:0];
}

void AInteractAnim::Trigger_Implementation() {
	Super::Trigger_Implementation();

	if (!AnimEnabled) return;
	// set the flag before so that the sound triggers are consistent
	IsOpen = !IsOpen;
	SetText(); // change the text after the flag has changed
	// this creates so many issues. notice how it's set.
	Animator->Play(!IsOpen);

	UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
}

bool AInteractAnim::TryTrigger_Implementation() {
	if (AnimEnabled && Animator->GetIsAnimating()) return false;
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
	SFX->SetSound(Snd);
	SFX->SetActive(true, true);
	SFX->Play(0);
}
