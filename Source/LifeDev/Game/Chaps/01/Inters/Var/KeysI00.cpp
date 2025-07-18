// Copyright Jerónimo Barraco-Mármol

#include "KeysI00.h"

AKeysI00::AKeysI00():Super() {
	StateNum = 1;
	UseAutoActivate = true;
	RewardFlash = .1;
	RewardItem = "KW0";
	UseFade = true; // needed to fade on reward
	UseAnim = false;
	UseHint = true;

	HintCondition = "{Inter.Locked.WDoorI00}";
}