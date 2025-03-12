// Copyright Jerónimo Barraco-Mármol

#include "PoemI02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

APoemI02::APoemI02():Super() {
	RewardFlash = -.2f;
	RewardItem = LDConsts::Items::Poem2;

	// Can't be static since it will be inside of a drawer
	Super::SetMobility(EComponentMobility::Movable);
}
