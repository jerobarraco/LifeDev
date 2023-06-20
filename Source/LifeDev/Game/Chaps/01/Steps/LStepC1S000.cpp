// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S000.h"

ALStepC1S000::ALStepC1S000():Super() {
	Name = FName("C1S0");
	DlgId = FName("C1S0"); // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = true;
	UseFadeTime = true;
	FinishPostWait = false; // will be set by seqid anyway
	CamTarget = nullptr; // use previous camera
}
