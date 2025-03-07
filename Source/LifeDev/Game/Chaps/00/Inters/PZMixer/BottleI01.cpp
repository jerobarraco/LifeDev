// Copyright Jerónimo Barraco-Mármol

#include "BottleI01.h"

#include "Interact/Animator/CAnimatorFade.h"

ABottleI01::ABottleI01():Super() {
	RewardItem = "Bottle01";
	UseRewardDestroy = true;
	UseFade = true;
	AnimFade->MatBase = nullptr;
	Texts = {
		NSLOCTEXT("Bottle00", "Text0", "Conditioner")
	};
}
