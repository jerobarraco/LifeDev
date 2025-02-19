// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S005.h"

ALStepC1S005::ALStepC1S005():Super() {
	Name = FName("C1S5");
	InputEnabled = true; // does it matters? it will show dialogs
	// uses pawn camera
	UsePawnCam = true;
	UseFadeTime = false;
	DlgId = "N01.0";
	FBDlgAutoTo = 1;
	UseFBDlgAuto = true;
	UseRain = false;
}

// TODO this one seems to take two seconds to start. not sure why.