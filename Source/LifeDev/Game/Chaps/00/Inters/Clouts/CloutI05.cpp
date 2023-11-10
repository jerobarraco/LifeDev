// Copyright Jerónimo Barraco-Mármol

#include "CloutI05.h"

ACloutI05::ACloutI05():Super() {
	TriggerDlg = "C05_T";
	RewardItem = "C1C01"; // reusing the chap01 for now.
	static FText T = FText::FromString(TEXT("Dirty Clothes"));
	Texts = { T };
}
