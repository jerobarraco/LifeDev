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
	AnimPlay();
}

void AInteractAnim::Trigger_Implementation() {
	// trigger the trigger sound and calls set text and sets the state. notice done after changing the state.
	Super::Trigger_Implementation(); // calling implementation to call the super
	// SetText(); // not needed. happens on super
}

bool AInteractAnim::TryTrigger_Implementation() {
	if (UseAnim && Anim->IsActive()) return false;
	return Super::TryTrigger_Implementation();
}

void AInteractAnim::AnimPlay() {
	if (!UseAnim) return;
	if (DisableWhileAnim) {
		SetEnabled(false);
	}

	// both checks avoids an out of bound access
	if (Trans.Num() ==0 || State < 0) {
		// this creates so many issues. notice how it's set.
		Anim->IsReversed = !IsOpen(); //!IsOpen== !(State!=0) 
	} else {
		// using troot since it could be changed in any child or parent
		Anim->TStart = Anim->TRoot->GetRelativeTransform();
		Anim->TEnd = Trans[State%Trans.Num()];
	}
	// not calling PlaySet on purpose. since that could break things like the light.
	// or if a child wants to do something weird.
	Anim->Play();
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
	UseAnim = Mobility == EComponentMobility::Movable;
}
