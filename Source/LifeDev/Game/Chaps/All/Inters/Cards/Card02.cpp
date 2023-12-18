// Copyright Jerónimo Barraco-Mármol

#include "Card02.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ACard02::ACard02():Super() {
	UseAnim = false;
	RewardItem = LDConsts::Items::Card2;
	Texts = { FText::FromString(TEXT("Pick Up")), FText::FromString(TEXT("Pick Up")) } ;
	TriggerDlg = "C02_Look*";
}
