// Copyright Jerónimo Barraco-Mármol

#include "PoemI03.h"

#include "LifeDev/Core/Consts/ConstItems.h"

APoemI03::APoemI03():Super() {
	RewardFlash = -.2f;
	RewardItem = LDConsts::Items::Poem3;
	UseAutoActivate = true;
	UseHint = true;
	Super::SetMobility(EComponentMobility::Static);
}
