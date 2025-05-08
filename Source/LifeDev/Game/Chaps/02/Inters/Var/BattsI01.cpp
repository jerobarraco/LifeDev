// Copyright Jerónimo Barraco-Mármol

#include "BattsI01.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ABattsI01::ABattsI01():Super() {
	Texts = { FText::FromString(TEXT("Pick up batteries")) };
	RewardItem = LDConsts::Items::Batts;

	// needs to move to be able to be attached to the drawer
	Super::SetMobility(EComponentMobility::Movable);
	Super::SetActorHiddenInGame(true);
}
