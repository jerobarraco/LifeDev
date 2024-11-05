// Copyright Jerónimo Barraco-Mármol

#include "PictureI00.h"

#include "Interact/Animator/CAnimatorMix.h"

APictureI00::APictureI00():Super() {
	UseRewardDestroy = false;
	UseFade = false;
	UseAnim = true;
	StateNum = 3;
	Texts = {
		FText::FromString(TEXT("Move")),
		FText::FromString(TEXT("Move again")),
		FText::FromString(TEXT("Move once more"))
	};
	const FRotator State0Rot(0,35.,0);
	const FRotator State1Rot(0,5,0);
	const FRotator State2Rot(0,-35.,0);
	Trans = {
		FTransform(State0Rot),
		FTransform(State1Rot),
		FTransform(State2Rot),
	};
	IRoot->SetRelativeTransform(Trans[0]);
	Anim->IsAdditive = false;
	// important. the puzzle will activate them.
	APictureI00::SetAutoActivate(false);
	APictureI00::SetMobility(EComponentMobility::Type::Movable); // super important
}
