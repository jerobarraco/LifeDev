// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S004.h"

ALStepC4S004::ALStepC4S004():Super() {
	Name = FName("C4S4");
	UseFade = false;
	UseRain = true; // TODO test

	// DlgId = FName("C4S1"); 
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

// enables the fridge and rice and pot