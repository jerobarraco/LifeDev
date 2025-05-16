// Copyright Jerónimo Barraco-Mármol

#include "PictureI00.h"

#include "Interact/Animator/CAnimatorMix.h"

APictureI00::APictureI00():Super() {
	UseRewardDestroy = false;
	UseFade = false;
	UseAnim = true;
	StateNum = 3;
	// important. the puzzle will activate them.
	UseAutoActivate = false; // redundant but, to be sure...
	SFXs = { SFXs[0], SFXs[0], SFXs[0] }; // expand to 3 states
	Texts = {
		NSLOCTEXT("PictureI00", "State0", "Move"),
		NSLOCTEXT("PictureI00", "State1", "Move again"),
		NSLOCTEXT("PictureI00", "State2", "Move once more")
	};
	const FRotator State0Rot(0,35.,0);
	const FRotator State1Rot(0,5,0);
	const FRotator State2Rot(0,-35.,0);
	Trans = {
		FTransform(State0Rot),
		FTransform(State1Rot),
		FTransform(State2Rot)};
	IRoot->SetRelativeTransform(Trans[0]);
	Anim->IsAdditive = false;
	Anim->Duration = 0; // see beginplay
	
	APictureI00::SetMobility(EComponentMobility::Type::Movable); // super important
}

void APictureI00::BeginPlay() {
	Super::BeginPlay();
	// small trick needed so that when the puzzle sets the state on begin play,
	// the animation is not noticeable. A duration of 0 in the constructor makes it instant.
	// (thanks to myself for supporting a duration of 0).
	// happens only because the camera is pointing right at it.
	// and we're using transform animations.
	
	Anim->Duration = 1;
}
