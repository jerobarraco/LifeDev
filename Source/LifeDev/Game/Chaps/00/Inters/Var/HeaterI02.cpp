// Copyright Jerónimo Barraco-Mármol

#include "HeaterI02.h"

AHeaterI02::AHeaterI02():Super() {
	UseAnim = false;
	Locked = true;
	UseAutoActivate = true;
	RewardFlash = .1;
	
	LockedDlg = "HT02_L";
	Texts = {
		NSLOCTEXT("HeaterI02", "TurnOn", "Turn On")};

	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
}
