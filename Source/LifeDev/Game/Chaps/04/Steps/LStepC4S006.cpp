// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S006.h"

ALStepC4S006::ALStepC4S006():Super() {
	Name = FName("C4S6");
	UseFade = false;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

// enables the sponge (set in editor)