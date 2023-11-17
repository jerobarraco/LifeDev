// Copyright Jerónimo Barraco-Mármol

#include "RadioI01.h"

ARadioI01::ARadioI01():Super() {
	// when user tries to play but has no tape
	LockedDlg = "RD01_L";
	Locked = true;
	StateNum = 1;
	Texts = {
		FText::FromString(TEXT("Noisy radio"))
	};
}
