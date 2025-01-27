// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "InteractAnim.h"

#include "Interact/CInteract.h"
#include "Animator/CAnimatorMix.h" // needed for Prim = Mesh. yes.

AInteractAnim::AInteractAnim():Super() {
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

void AInteractAnim::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	// play the animation, since we want to make sure it ends where it needs to
	AnimPlay();
}

bool AInteractAnim::TryTrigger_Implementation() {
	// cancel re-trigger if it's busy.
	if (UseAnim && Anim->IsActive()) return false;
	return Super::TryTrigger_Implementation();
}

void AInteractAnim::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	// disable disableWhileAnim if this is one shot. otherwise it will try to re-enable
	// do after Trigger, so it actually disable during the animation
	// also not doing during SetState since that can also be called by other means.
	// done here and not on AnimEnd due to the same reason.
	if (IsOneShot) DisableWhileAnim = false;

	// OnTriggerDone is dispatched on AnimEnd. but if it's not being used. we force it.
	if (!UseAnim || !Anim->IsActive()) DoTriggerAnim();
}

void AInteractAnim::DoTriggerAnim() {
	OnTriggerAnim.Broadcast();
}

void AInteractAnim::AnimPlay() {
	if (!UseAnim) return;

	if (DisableWhileAnim) SetActive(false);

	// both checks avoid an out of bound access
	if (Trans.Num() == 0 || State < 0) {
		// this creates so many issues. notice how it's set.
		// this plays AFTER the state has changed.
		// which means: if it's open, it was closed, so it needs to play from Closed to Open
		// closed means Alpha 0, open is Alpha 1 (0= left 1= right on the curve)
		// Not reversed is 0 to 1 (close->Open), reversed is 1 to 0 (open->Close)
		Anim->IsReversed = IsReversed();
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
	if (DisableWhileAnim) SetActive(true);

	if (State >= 0 && State < SFX_Stop.Num()) {
		USoundBase* const Snd2 = SFX_Stop[State];
		PlaySFX(Snd2);
	}

	OnTriggerAnim.Broadcast();
}

// void AInteractAnim::SetMobility(EComponentMobility::Type Mobility) {
	// Super::SetMobility(Mobility);
	// this optimization is prone to create issues if we change to Movable yet we still want to
	// disable UseAnim. Since UseAnim is not mutual implication with mobility. then it's better not to.
	// UseAnim = Mobility == EComponentMobility::Movable;
