// Copyright Jerónimo Barraco-Mármol

#include "PoemI02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

APoemI02::APoemI02():Super() {
	UseHint = true;
	RewardFlash = -.2f;
	RewardItem = LDConsts::Items::Poem2;
	Super::SetActorHiddenInGame(true); // made visible by step. this triggers a call to setActive(false) but it's ok.
	// Can't be static since it will be inside a drawer
	Super::SetMobility(EComponentMobility::Movable);
}
