// Copyright Jerónimo Barraco-Mármol

#include "HeaterI01.h"
#include "Interact/CInteract.h"

AHeaterI01::AHeaterI01():Super() {
	UseAnim = false;
	Locked = true;
	Interact->SetEnabled(true);
	
	LockedDlg = "HT01_L";
	Texts = {
		FText::FromString(TEXT("Turn on")),
	};
}
