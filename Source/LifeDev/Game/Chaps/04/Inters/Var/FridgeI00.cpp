// Copyright Jerónimo Barraco-Mármol

#include "FridgeI00.h"

AFridgeI00::AFridgeI00():Super() {
	IsOneShot=true;
	UseAutoActivate = true;
	UseRewardDestroy = false;
	RewardItem = "Food01";
	UseFade = false;
	UseHint = true;
	HintCondition = "{Inter.Locked.PotI00}";
	// Locked=true;
	// TriggerDlg="Fridge00_T";
	// RewardFlash = .1;
}
