// Copyright Jerónimo Barraco-Mármol

#include "CloutI06.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACloutI06::ACloutI06():Super() {
	TriggerDlg = "C06_T";
	RewardItem = LDConsts::Items::Bra; // reusing the chap01 for now
	static FText T = FText::FromString(TEXT("Dirty Clothes"));
	Texts = { T };
}
