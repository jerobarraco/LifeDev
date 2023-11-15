// Copyright Jerónimo Barraco-Mármol

#include "RadioI00.h"

ARadioI00::ARadioI00():Super() {
	// when user tries to play but has no tape
	LockedDlg = "RD00_L";
	// when user tries to play but has tape
	LockedItemDlg = "RD00_LI";
	// when user unlocks using the tape
	ULockDlg = "RD00_UL";
	// when the user activates. though this happens automatically after ULockDlg.
	//  it's all ok though, it's according to keikaku (keikaku means plan).
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
