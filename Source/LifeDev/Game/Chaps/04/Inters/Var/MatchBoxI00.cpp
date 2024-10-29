// Copyright Jerónimo Barraco-Mármol

#include "MatchBoxI00.h"

AMatchBoxI00::AMatchBoxI00():Super() {
	RewardItem = "Matches00";
	UseRewardDestroy = true;
	UseFade = true;
	// TriggerDlg = "Food00_T";
	// RewardFlash = .1; // story works better this way
	// Locked = true;
	// LockedDlg = "Food00_L";
	Super::SetAutoActivate(true);
}
