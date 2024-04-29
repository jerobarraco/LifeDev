// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S002.h"

ALStepC4S002::ALStepC4S002():Super() {
	Name = FName("C4S2");
	UseFade = false;
	// DlgId = FName("C4S1"); 
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
	UseRain = true; // TODO test
}

// enables the pot