// Copyright Jerónimo Barraco-Mármol

#include "InteractAnim.h"

#include "Interact/CInteract.h"
#include "Animator/CAnimatorMix.h"

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
	Anim->OnBegin.RemoveAll(this);
	Anim->OnEnd.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void AInteractAnim::SetText_Implementation() {
	Super::SetText_Implementation(); // useless

	if (Texts.Num()<1) {
		UE_LOG(LogTemp, Warning, TEXT("AInteractAnim.SetText: Object has no text to set"));
		return;
	}

	Interact->Text = Texts[State%Texts.Num()];
	// if (Texts.Num()<2) {
		// Interact->Text = Texts[0];
		// return;
	// }
	
	// Interact->Text = Texts[IsOpen?1:0];
	UE_LOG(LogTemp, Log, TEXT("AInteractAnim.SetText: NewText=%s"), *Interact->Text.ToString());
}

void AInteractAnim::Trigger_Implementation() {
	// set the flag before so that the sound triggers are consistent
	IsOpen = !IsOpen;
	State = (State +1) % StateCount;
	UE_LOG(LogTemp, Log, TEXT("AInteractAnim.Trigger: State=%i"), State);

	if (AnimEnabled) {
		// this creates so many issues. notice how it's set.
		Anim->PlaySet(State==0); //!IsOpen);
		UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
	}
	// trigger the trigger sound and calls set text. notice done after changing the state.
	Super::Trigger_Implementation();
	// SetText(); // not needed. happens on super
}

bool AInteractAnim::TryTrigger_Implementation() {
	if (AnimEnabled && Anim->IsActive()) return false;
	return Super::TryTrigger_Implementation();
}

void AInteractAnim::AnimBegin_Implementation() {
	// at this point the isOpen flag is toggled
	USoundBase* const Snd = IsOpen ? SFX_Open : SFX_Close; // todo remove
	PlaySFX(Snd);
	
	if (State<0 || State >= SFX_Start.Num()) return;
	USoundBase* const Snd2 = SFX_Start[State];
	PlaySFX(Snd2);
}

void AInteractAnim::AnimEnd_Implementation() {
	// at this point the isOpen flag is toggled
	USoundBase* const Snd = IsOpen ? SFX_OpenEnd : SFX_CloseEnd; // todo remove
	PlaySFX(Snd);

	if (State<0 || State >= SFX_Stop.Num()) return;
	USoundBase* const Snd2 = SFX_Stop[State];
	PlaySFX(Snd2);
}

void AInteractAnim::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	AnimEnabled = Mobility == EComponentMobility::Movable;
}
