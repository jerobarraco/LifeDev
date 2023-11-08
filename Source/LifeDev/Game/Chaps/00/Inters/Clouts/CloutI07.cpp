// Copyright Jerónimo Barraco-Mármol

#include "CloutI07.h"

ACloutI07::ACloutI07():Super() {
	TriggerDlg = "C07_T";
	ItemReward = "C1C04"; // reusing the chap01 for now
	static FText T = FText::FromString(TEXT("Dirty Clothes"));
	Texts = { T };
}
