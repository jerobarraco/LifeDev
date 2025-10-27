// Copyright Jerónimo Barraco-Mármol

#include "PoemI02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

APoemI02::APoemI02():Super() {
	// UseHint = false; // hint is set on the step that waits for it.
	RewardFlash = -.2f;
	RewardItem = LDConsts::Items::Poem2;
	UseAutoActivate = true;
	UseHint = true;
	HintCondition = "{Step.Start.C2S4}"; // when reaching step 3 hint. as the step waits for it.
	// it's active since the start.
	// Super::SetActorHiddenInGame(true); // made visible by step. this triggers a call to setActive(false) but it's ok.
	// Can't be static since it will be inside a drawer
	Super::SetMobility(EComponentMobility::Movable);
}
