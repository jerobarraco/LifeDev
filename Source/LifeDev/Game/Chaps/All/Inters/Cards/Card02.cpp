// Copyright Jerónimo Barraco-Mármol

#include "Card02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard02::ACard02():Super() {
	RewardItem = LDConsts::Items::Card2;
	UseHint = true;
	HintCondition = "{Step.Start.C2S4}";
}
