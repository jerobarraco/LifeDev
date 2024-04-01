// Copyright Jerónimo Barraco-Mármol

#include "PotI00.h"

APotI00::APotI00():Super() {
	RewardFlash = 0.1;
	UseRewardFade = false;
	Locked = true;
	LockedDlg = "Pot00_L";

	SetState(1); // start open
}

