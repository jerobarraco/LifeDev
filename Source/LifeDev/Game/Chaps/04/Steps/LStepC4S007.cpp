// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S007.h"

ALStepC4S007::ALStepC4S007():Super() {
	Name = FName("C4S7");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;
	TeleportChar = false;
}

// interfadeout on editor