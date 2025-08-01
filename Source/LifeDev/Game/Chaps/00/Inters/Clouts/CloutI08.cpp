// Copyright Jerónimo Barraco-Mármol

#include "CloutI08.h"

ACloutI08::ACloutI08():Super() {
	// TriggerDlg = "C08_T"; // todo add. this doesn't exists
	RewardItem = "C1C02"; // reusing the chap01 for now
	static FText T = NSLOCTEXT("CloutI08", "State0", "Dirty Clothes");
	Texts = { T };
}
