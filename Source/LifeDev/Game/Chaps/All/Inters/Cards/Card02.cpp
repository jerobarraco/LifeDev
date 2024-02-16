// Copyright Jerónimo Barraco-Mármol

#include "Card02.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ACard02::ACard02():Super() {
	RewardItem = LDConsts::Items::Card2;
	TriggerDlg = "C2_Look*";
}
