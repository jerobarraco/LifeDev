// Copyright Jerónimo Barraco-Mármol

#include "HeaterI02.h"
#include "Interact/CInteract.h"

AHeaterI02::AHeaterI02():Super() {
	UseAnim = false;
	Locked = true;
	
	LockedDlg = "HT02_L";
	Texts = {
		FText::FromString(TEXT("Turn on")),
	};
	RewardFlash = .1;

	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
	Super::SetAutoActivate(true);
}
