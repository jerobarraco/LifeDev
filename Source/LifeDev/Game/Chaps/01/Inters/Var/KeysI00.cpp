// Copyright Jerónimo Barraco-Mármol

#include "KeysI00.h"

AKeysI00::AKeysI00():Super() {
	Texts = { NSLOCTEXT("KeysI00", "State0", "Keys")};
	RewardFlash = .1;
	RewardItem = "KW0";
	UseFade = true; // needed to fade on reward
	UseAnim = false;
	StateNum = 1;
	UseAutoActivate = true;
}