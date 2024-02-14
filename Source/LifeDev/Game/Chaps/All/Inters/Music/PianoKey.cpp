// Copyright Jerónimo Barraco-Mármol

#include "PianoKey.h"

#include "Interact/Animator/CAnimatorMix.h"

APianoKey::APianoKey():Super() {
	SFX_Trigger = nullptr; // TODO

	UseAnim = true;
	Anim->TEnd.SetRotation(FRotator(0,0,10).Quaternion());
}
