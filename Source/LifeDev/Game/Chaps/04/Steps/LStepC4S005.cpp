// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S005.h"

ALStepC4S005::ALStepC4S005():Super() {
	Name = FName("C4S5");
	DlgId = Name; // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	TeleportChar = true;
	
	UseFBDlgAuto = true;
	FBDlgAutoTo = 1.2;
}

// interfadeout on editor
// todo move here