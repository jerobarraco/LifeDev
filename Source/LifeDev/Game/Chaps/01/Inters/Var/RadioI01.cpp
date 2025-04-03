// Copyright Jerónimo Barraco-Mármol

#include "RadioI01.h"

ARadioI01::ARadioI01():Super() {
	// when user tries to play but has no tape
	LockedDlg = "RD01_L";
	Locked = true;
	UseAutoActivate = true;
	StateNum = 1;
	Texts = {
		NSLOCTEXT("RadioI01", "State0", "Noisy radio")};
}
