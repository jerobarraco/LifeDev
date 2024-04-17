// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S005.h"

ALStepC4S005::ALStepC4S005():Super() {
	Name = FName("C4S5");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	// WaitTime = CamBlendTime = 2;
	// set wait to blend so that FIRST you turn and THEN it fades the chars (on DoStart (after wait))
	FinishPostWait = false;
	TeleportChar = false;
}

// interfadeout on editor