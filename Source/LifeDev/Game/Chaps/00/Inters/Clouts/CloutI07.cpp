// Copyright Jerónimo Barraco-Mármol

#include "CloutI07.h"

ACloutI07::ACloutI07():Super() {
	// TriggerDlg = "C07_T"; // TODO have to add it back it doesn't exists
	RewardItem = "C1C04"; // reusing the chap01 for now
	static FText T = NSLOCTEXT("CloutI07", "State0", "Dirty Clothes");
	Texts = { T };
}
