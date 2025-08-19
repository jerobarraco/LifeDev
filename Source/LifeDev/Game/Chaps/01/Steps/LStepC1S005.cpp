// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S005.h"

ALStepC1S005::ALStepC1S005():Super() {
	Name = FName("C1S5");
	InputEnabled = true; // does it matter? it will show dialogs.
	// DlgId = "N01.0";
	FBDlgAutoTo = 1;
	UseFBDlgAuto = true;
	UseFBAnim = true;
	UseRain = false;
	UsePawnCam = false;
	// WaitTime = CamBlendTime = 0; // avoid extra wait
}
