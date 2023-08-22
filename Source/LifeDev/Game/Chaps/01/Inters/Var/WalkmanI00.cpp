// Copyright Jerónimo Barraco-Mármol

#include "WalkmanI00.h"
#include "Interact/CInteract.h"

AWalkmanI00::AWalkmanI00():Super() {
	AnimEnabled = false;
	Interact->SetEnabled(true);
	
	TriggerFlashInc = .1f;
	TriggerDlg = "WM00_T"; // TODO
	LockedDlg = "WM00_L"; // TODO
	ItemReward = "WM00";// TODO
	Locked = true;
	ULockItem = "Batts";
	Texts = {
		FText::FromString(TEXT("A walkman with no batteries")),
	};
	// TODO set up meshes
}

