// Copyright Jerónimo Barraco-Mármol

#include "HeaterI03.h"

AHeaterI03::AHeaterI03():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	RewardFlash = .1;
	IsOneShot = true;
	IsLocked = false;
	UseAutoActivate = true;
	// LockedDlg = "HT03_L";
	// TriggerDlg = "HT03_L";
	Texts = { NSLOCTEXT("HeaterI00", "State0", "Turn on") };
	StateNum = 1;
}
