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
	UE_LOG(LogTemp, Log, TEXT("AInteractAnim.SetText: State=%i, NewText=%s"), State, *Interact->Text.ToString());
}

void AInteractAnim::SetState_Implementation(int32 NewState) {
	Super::SetState_Implementation(NewState);
	// play the animation, since we want to make sure it ends where it needs to
	AnimPlay();
}

bool AInteractAnim::TryTrigger_Implementation() {
	// don't re-trigger if it's busy.
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
		// this plays AFTER the state has changed.
		// which means: if it's open, it was closed, so it needs to play from Closed to Open
		// closed means Alpha 0, open is Alpha 1 (0= left 1= right on the curve)
		// Not reversed is 0 to 1 (close->Open), reversed is 1 to 0 (open->Close)
		Anim->IsReversed = IsClosed();
	} else {
		// using troot since it could be changed in any child or parent
		Anim->TStart = Anim->TRoot->GetRelativeTransform();
		Anim->TEnd = Trans[State%Trans.Num()];
	}

	// not calling PlaySet on purpose. since that could break things like the light.
	// or if a child wants to do something weird.
	Anim->Activate(true);
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
