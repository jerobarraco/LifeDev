// Copyright Jerónimo Barraco-Mármol

#include "Card03.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard03::ACard03():Super() {
	// overriden description for this chapter. will show on trigger.
	RewardItem = LDConsts::Items::Card3;
	Super::SetMobility(EComponentMobility::Static);
	UseHint = true;
}
