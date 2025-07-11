// Copyright Jerónimo Barraco-Mármol

#include "Card02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard02::ACard02():Super() {
	UseHint = false; // hinted by step
	RewardItem = LDConsts::Items::Card2;
}
