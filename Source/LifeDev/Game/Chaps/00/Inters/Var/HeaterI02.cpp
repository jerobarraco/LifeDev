// Copyright Jerónimo Barraco-Mármol

#include "HeaterI02.h"
#include "Interact/CInteract.h"

AHeaterI02::AHeaterI02():Super() {
	UseAnim = false;
	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
	Locked = true;
	Interact->SetActive(true);
	
	LockedDlg = "HT02_L";
	Texts = {
		FText::FromString(TEXT("Turn on")),
	};
	RewardFlash = .1;
}
