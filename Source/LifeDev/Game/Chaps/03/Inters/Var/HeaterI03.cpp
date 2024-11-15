// Copyright Jerónimo Barraco-Mármol

#include "HeaterI03.h"

AHeaterI03::AHeaterI03():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	RewardFlash = .1;
	IsOneShot = true;
	Locked = false;
	// LockedDlg = "HT03_L";
	TriggerDlg = "HT03_L"; // TODO rename
	Texts = { FText::FromString(TEXT("Turn on")) };

	Super::SetAutoActivate(true);
}
