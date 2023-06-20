// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S000.h"

ALStepC1S000::ALStepC1S000():Super() {
	Name = FName("C1S0");
	SeqId = FName("C1S0");
	InputEnabled = false;
	UsePawnCam = true;
	UseCam = false;
	UseFadeTime = true;

	// finish on obtaining these.
	ItemsPass = {
		"T0",
		"Walkman"
	};
}
