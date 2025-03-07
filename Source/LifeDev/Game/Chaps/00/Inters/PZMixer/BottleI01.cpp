// Copyright Jerónimo Barraco-Mármol

#include "BottleI01.h"

ABottleI01::ABottleI01():Super() {
	RewardItem = "Bottle01";
	UseRewardDestroy = true;
	UseFade = true;
	
	Texts = {
		NSLOCTEXT("Bottle00", "Text0", "Conditioner")
	};
}
