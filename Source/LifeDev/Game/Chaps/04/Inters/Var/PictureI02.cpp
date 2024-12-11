// Copyright Jerónimo Barraco-Mármol

#include "PictureI02.h"

#include "Interact/Animator/CAnimatorMix.h"

APictureI02::APictureI02():Super() {
	UseRewardDestroy = false;
	UseFade = false;
	UseAnim = true;
	StateNum = 1;
	TriggerDlg = "Pic02_T";
	Texts = {
		FText::FromString(TEXT("Picture")),
		// FText::FromString(TEXT("Move")),
		// FText::FromString(TEXT("Move again")),
		// FText::FromString(TEXT("Move once more"))
	};
	const FRotator State0Rot(0,35.,0);
	// const FRotator State1Rot(0,5,0);
	// const FRotator State2Rot(0,-35.,0);
	Trans = {
		FTransform(State0Rot),
		// FTransform(State1Rot),
		// FTransform(State2Rot),
	};
	IRoot->SetRelativeTransform(Trans[0]);
	Anim->IsAdditive = false;
	// Anim->Duration = 0; // see beginplay
	// important. the puzzle will activate them.
	APictureI02::SetAutoActivate(true);
	APictureI02::SetMobility(EComponentMobility::Type::Movable); // super important
}
