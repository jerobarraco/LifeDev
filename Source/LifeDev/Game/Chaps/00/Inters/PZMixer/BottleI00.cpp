// Copyright Jerónimo Barraco-Mármol

#include "BottleI00.h"

#include "Interact/Animator/CAnimatorFade.h"

ABottleI00::ABottleI00():Super() {
	RewardItem = "Bottle00";
	UseRewardDestroy = true;
	UseFade = true;
	AnimFade->MatBase = nullptr;
}
