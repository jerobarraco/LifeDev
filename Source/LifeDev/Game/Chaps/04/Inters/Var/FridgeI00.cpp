// Copyright Jerónimo Barraco-Mármol

#include "FridgeI00.h"

AFridgeI00::AFridgeI00():Super() {
	UseAutoActivate = true;
	UseRewardDestroy = false;
	RewardItem = "Food01";
	UseFade = false;
	UseHint = true;
	HintCondition = "{Inter.Locked.PotI00}"; // by default it stops hinting after trigger.
	IsOneShot = false; // we will lock it after
	
	// Locked=true;
	// TriggerDlg="Fridge00_T";
	// RewardFlash = .1;
}

void AFridgeI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	IsLocked = true; // lock
}
