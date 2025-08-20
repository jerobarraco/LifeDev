// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S007.h"

ALStepC4S007::ALStepC4S007():Super() {
	Label = FName("C4S7");
	// DlgId = Name; // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	TeleportChar = false;
	UseFBDlgAuto = true;
	FBDlgAutoTo = 1.2;
}

// interfadeout on editor
