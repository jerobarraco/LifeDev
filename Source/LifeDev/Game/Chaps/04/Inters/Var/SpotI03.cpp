// Copyright Jerónimo Barraco-Mármol

#include "SpotI03.h"

ASpotI03::ASpotI03():Super() {
	// DropDlg = TEXT("IS0_D*");
	TriggerDlg = "IS3_T"; // when completed
	LockedDlg = "IS3_L"; // when not full
	DropFullDlg = ""; // when full
	RewardStep = true;
	Items = {
		// a bra and a shirt a boxer and a sock
		FName("Plates")
	};
	// RewardActor set in editor
}
