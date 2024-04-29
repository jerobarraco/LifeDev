// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S002.h"

ALStepC4S002::ALStepC4S002():Super() {
	Name = FName("C4S2");
	// DlgId = FName("C4S1"); 
	UseFade = false;
	UseRain = true;
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

// enables the pot