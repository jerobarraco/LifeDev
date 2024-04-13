// Copyright Jerónimo Barraco-Mármol

#include "FridgeI00.h"

AFridgeI00::AFridgeI00():Super() {
	// Locked=true;
	IsOneShot=true;
	TriggerDlg="Fridge00_T";
	UseRewardFade = false;
	RewardItem = "Food01";
	RewardFlash = .1;
	SetEnabled(false);
}

