// Copyright Jerónimo Barraco-Mármol

#include "Card02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard02::ACard02():Super() {
	UseHint = true; // hint as soon as it appears
	RewardItem = LDConsts::Items::Card2;
}
