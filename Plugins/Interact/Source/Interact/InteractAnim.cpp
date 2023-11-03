// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

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

	if (State < 0) {
		UE_LOG(LogTemp, Log, TEXT("AInteractAnim.SetText: Can't set text with invalid state=%i"), State);
		return;
	}

	Interact->Text = Texts[State%Texts.Num()];
	UE_LOG(LogTemp, Log, TEXT("AInteractAnim.SetText: NewText=%s"), *Interact->Text.ToString());
}

void AInteractAnim::SetState_Implementation(int32 NewState) {
	Super::SetState_Implementation(NewState);

	if (!AnimEnabled) return;
	if (DisableWhileAnim) {
		SetEnabled(false);
	}

	// both checks avoids an out of bound access
	if (Trans.Num()==0 || State < 0) {
		// this creates so many issues. notice how it's set.
		Anim->PlaySet(!GetIsOpen()); //!IsOpen); !(State!=0)
	} else {
		// using troot since it could be changed in any child or parent
		Anim->TStart = Anim->TRoot->GetRelativeTransform();
		Anim->TEnd = Trans[State%Trans.Num()];
		Anim->PlaySet();
	}
}

void AInteractAnim::Trigger_Implementation() {
	// set the flag before so that the sound triggers are consistent
	const int32 NewState = (State +1) % StateNum;
	UE_LOG(LogTemp, Log, TEXT("InteractAnim.Trigger: open=%i, state=%i"), GetIsOpen() ? 0:1, NewState);

	SetState(NewState);

	// trigger the trigger sound and calls set text. notice done after changing the state.
	Super::Trigger_Implementation();
	// SetText(); // not needed. happens on super
}

bool AInteractAnim::TryTrigger_Implementation() {
	if (AnimEnabled && Anim->IsActive()) return false;
	return Super::TryTrigger_Implementation();
}

void AInteractAnim::AnimBegin_Implementation() {
	// at this point the state ( isOpen ) flag is toggled
	
	if (State < 0 || State >= SFX_Start.Num()) return;
	USoundBase* const Snd2 = SFX_Start[State];
	PlaySFX(Snd2);
}

void AInteractAnim::AnimEnd_Implementation() {
	// at this point the state ( isOpen ) flag is toggled
	if (DisableWhileAnim) {
		SetEnabled(true);
	}
	
	if (State<0 || State >= SFX_Stop.Num()) return;
	USoundBase* const Snd2 = SFX_Stop[State];
	PlaySFX(Snd2);
}

void AInteractAnim::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	AnimEnabled = Mobility == EComponentMobility::Movable;
}
