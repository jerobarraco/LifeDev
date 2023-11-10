// Copyright Jerónimo Barraco-Mármol

#include "CloutI08.h"

ACloutI08::ACloutI08():Super() {
	TriggerDlg = "C08_T";
	RewardItem = "C1C02"; // reusing the chap01 for now
	static FText T = FText::FromString(TEXT("Dirty Clothes"));
	Texts = { T };
}
