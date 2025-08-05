// Copyright Jerónimo Barraco-Mármol

#include "Card04.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard04::ACard04():Super() {
	// overriden description for this chapter. will show on trigger.
	RewardItem = LDConsts::Items::Card4;
	UseHint = true; // show hints by default
	// the step will finish on this, set on the step.
} // not active by default. can't remember why. maybe a step.
