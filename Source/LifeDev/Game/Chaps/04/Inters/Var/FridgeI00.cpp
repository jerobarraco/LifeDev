// Copyright Jerónimo Barraco-Mármol

#include "FridgeI00.h"

AFridgeI00::AFridgeI00():Super() {
	// Locked=true;
	IsOneShot=true;
	TriggerDlg="Fridge00_T";
	UseRewardDestroy = false;
	RewardItem = "Food01";
	UseFade = false;
	// RewardFlash = .1;

	Super::SetAutoActivate(true);
}
