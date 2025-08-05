// Copyright Jerónimo Barraco-Mármol

#include "Card01.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard01::ACard01():Super() {
	RewardItem =  LDConsts::Items::Card1;
	// TriggerDlg = "C1_PICK";
	UseHint = true; // easiest way to do it. it will hint as soon as is shown
}
