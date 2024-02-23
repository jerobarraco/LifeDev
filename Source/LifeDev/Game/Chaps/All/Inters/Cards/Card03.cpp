// Copyright Jerónimo Barraco-Mármol

#include "Card03.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ACard03::ACard03():Super() {
	TriggerDlg = "C03_Look*";
	RewardItem = LDConsts::Items::Card3;
	UseRewardFade = true;
	Super::SetMobility(EComponentMobility::Static);
}
