// Copyright Jerónimo Barraco-Mármol

#include "HeaterI03.h"

AHeaterI03::AHeaterI03():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	RewardFlash = .1;
	IsOneShot = true;
	Locked = true;
	LockedDlg = "HT03_L";
	Texts = { FText::FromString(TEXT("Turn on")) };

	Super::SetAutoActivate(true);
}
