// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S006.h"

ALStepC1S006::ALStepC1S006():Super() {
	Label = FName("C1S6");
	InputEnabled = true; // does it matter? it will show dialogs
	// uses pawn camera
	CamTarget = this;
	UsePawnCam = false;
	// DlgId = "N01.1";
	FBDlgAutoTo = .5;
	UseFBDlgAuto = true;
	UseFBAnim = true;
	UseRain = true;
	TeleportChar=true;
}
