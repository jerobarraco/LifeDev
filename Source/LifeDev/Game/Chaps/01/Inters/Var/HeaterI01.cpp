// Copyright Jerónimo Barraco-Mármol

#include "HeaterI01.h"

AHeaterI01::AHeaterI01():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	Locked = true;
	
	LockedDlg = "HT01_L";
	Texts = { FText::FromString(TEXT("Turn on")) };

	Super::SetAutoActivate(true);
}
