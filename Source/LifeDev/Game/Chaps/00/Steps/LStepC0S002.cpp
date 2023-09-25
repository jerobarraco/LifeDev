// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S002.h"

ALStepC0S002::ALStepC0S002():Super() {
	Name = FName("C0S2");
	InputEnabled = false;
	UsePawnCam = false;
	CamTarget = nullptr; // will keep the current camera
	WaitTime = 2;
	FinishPostWait = true;
}