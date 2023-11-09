// Copyright Jerónimo Barraco-Mármol

#include "Card00.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ACard00::ACard00():Super() {
	ItemReward = LDConsts::Items::Card0;
	TriggerDlg = "C0_PICK";
}
