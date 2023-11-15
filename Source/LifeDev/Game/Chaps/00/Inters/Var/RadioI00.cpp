// Copyright Jerónimo Barraco-Mármol

#include "RadioI00.h"

ARadioI00::ARadioI00():Super() {
	LockedDlg = "RD00_L";
	LockedItemDlg = "RD00_LI";
	ULockDlg = "RD00_UL";
	TriggerDlg = "RD00_T";
	ULockItem = "T01"; // Tape
	DisableWhileAnim = false; // to allow to disable once activated
	Texts = {
		FText::FromString(TEXT("Play"))
	};
}

void ARadioI00::Trigger_Implementation() {
	Super::Trigger_Implementation();
	SetEnabled(false);// disable once activated. one use only.
}
