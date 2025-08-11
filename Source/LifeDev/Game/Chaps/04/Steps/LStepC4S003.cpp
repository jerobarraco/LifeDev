// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S003.h"

ALStepC4S003::ALStepC4S003():Super() {
	Name = FName("C4S3");
	DlgId = Name; // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	// go a bit above to ensure we end up at 1 a couple of dialogs before
	FBDlgAutoTo = 1.2;
	UseFBDlgAuto = true;
	TeleportChar = true;
}

// interfadeout in editor
// todo move here
