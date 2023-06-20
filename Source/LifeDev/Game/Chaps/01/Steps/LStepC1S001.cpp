// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S001.h"

ALStepC1S001::ALStepC1S001():Super() {
	Name = FName("C1S1");
	InputEnabled = true;
	UsePawnCam = true;
	
	// finish on obtaining these.
	FinishItems = {
		"T0",
		"Walkman"
	};
}

