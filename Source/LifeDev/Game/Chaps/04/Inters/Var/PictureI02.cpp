// Copyright Jerónimo Barraco-Mármol

#include "PictureI02.h"

#include "Interact/Animator/CAnimatorMix.h"

APictureI02::APictureI02():Super() {
	UseAutoActivate = true;
	UseRewardDestroy = false;
	UseFade = false;
	UseAnim = true; // TODO this is weird with 1 state.
	StateNum = 1;
	// TriggerDlg = "Pic02_T";

	const FRotator State0Rot(0,35.,0);
	Trans = { FTransform(State0Rot) };
	IRoot->SetRelativeTransform(Trans[0]);
	Anim->IsAdditive = false;

	APictureI02::SetMobility(EComponentMobility::Type::Movable); // super important
}
