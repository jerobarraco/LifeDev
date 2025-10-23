// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "InteractAnim.h"

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

void AInteractAnim::SetStateNow_Implementation(const int32 NewState, const bool UseSFX, const bool UseParts) {
	Super::SetStateNow_Implementation(NewState, UseSFX, UseParts);

	AnimSet();
	// "Is this too much voodoo?" -- Terry Davis
	// usually i would put this inside the previous func, but since AnimPlay also calls it,
	// i'd need to add an if, that's going to fail some of the times.
	// since it's only 1 LOC, and it's a private function, and only called here,
	// this is a better place for it. (it's also the same pattern in AnimPlay)
	// force to perform the thing. account for reversed.
	if (LIKELY(Anim))
		Anim->Update(Anim->IsReversed ? 0: 1);
}

void AInteractAnim::SetState_Implementation(const int32 NewState) { // called by dotrigger
	// this is a bit ugly for my taste, but it's the best place for now.
	// has to happen before SetState. since it will rumble. It also has to check for NewState.
	// i can't do it before because the user can change the duration at any moment.
	// though after testing i don't notice any difference. it might be because i'm using embedded curves instead of curve assets.
	if (UseAnim & LIKELY((NewState >=0)) & (NewState<Rumbles.Num()))
		Rumbles[NewState]->Duration = Anim->Duration;

	Super::SetState_Implementation(NewState);
	// play the animation, since we want to make sure it ends where it needs to
	AnimPlay();
}

bool AInteractAnim::TryTrigger_Implementation() {
	// cancel re-trigger if it's busy.
	if (UNLIKELY(UseAnim & Anim->IsActive())) return false;

	return Super::TryTrigger_Implementation();
}

void AInteractAnim::DoTrigger_Implementation() { // called by trytrigger
	// disable disableWhileAnim if this is one shot. otherwise it will try to re-enable
	// do after Trigger, so it actually disable during the animation
	// also not doing during SetState since that can also be called by other means.
	// done here and not on AnimEnd due to the same reason.
	if ((UseAnim & DisableWhileAnim) | IsOneShot) SetActive(false);
	Super::DoTrigger_Implementation();
}

void AInteractAnim::AnimPlay() { // called by setstate. called by dotrigger.
	if (!UseAnim | UNLIKELY(!Anim)) {
		// OnTriggerAnim is dispatched on AnimEnd. but if it's not being used. we force it.
		// the anim is triggered by setstate.
		// Calling AnimEnd has some other side effects. like playing sounds. which, while odd, i think it's benign side effect.
		AnimEnd();
		return;
	}

	AnimSet();

	// not calling PlaySet on purpose. since that could break things like the light.
	// or if a child wants to do something weird.
	Anim->Activate(true);
}

void AInteractAnim::AnimSet() {
	if (!UseAnim | UNLIKELY(!Anim)) return;
	// important not to check for troot here for things like lights
	
	// both checks avoid an out of bound access
	if (Trans.Num() == 0) {
		// this creates so many issues. notice how it's set.
		// this plays AFTER the state has changed.
		// which means: if it's open, it was closed, so it needs to play from Closed to Open
		// closed means Alpha 0, open is Alpha 1 (0= left 1= right on the curve)
		// Not reversed is 0 to 1 (close->Open), reversed is 1 to 0 (open->Close)
		Anim->IsReversed = IsEven(); // IsReversed();
	} else {
		// using troot since it could be changed in any child or parent
		// this will reset to the current place. so that the transition is always from Current to New
		// this is always IsReversed = false so it's always animating towards the new transform
		Anim->CopyTStart();
		Anim->TEnd = Trans[State%Trans.Num()];
	}
}

void AInteractAnim::AnimBegin_Implementation() {
	// at this point the state ( isOpen ) flag is toggled
}

void AInteractAnim::AnimEnd_Implementation() {
	// at this point the state ( isOpen ) flag is toggled
	if ((UseAnim & DisableWhileAnim) & !IsOneShot) SetActive(true);

	if (LIKELY(State >= 0) & (State < SFX_Stop.Num())) {
		USoundBase* const Snd2 = SFX_Stop[State];
		PlaySFX(Snd2);
	}

	OnAnimEnd.Broadcast();
}

// void AInteractAnim::SetMobility(EComponentMobility::Type Mobility) {
	// Super::SetMobility(Mobility);
	// this optimization is prone to create issues if we change to Movable yet we still want to
	// disable UseAnim. Since UseAnim is not mutual implication with mobility. then it's better not to.
	// UseAnim = Mobility == EComponentMobility::Movable;
