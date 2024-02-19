// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S001.h"

ALStepC3S001::ALStepC3S001():Super() {
	Name = FName("C3S1");
	UseFade = false;
	// DlgId = FName("C3S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = false;
	FinishPostWait = false;
}
