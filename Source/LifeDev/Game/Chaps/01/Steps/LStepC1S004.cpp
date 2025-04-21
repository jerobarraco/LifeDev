// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S004.h"

ALStepC1S004::ALStepC1S004():Super() {
	Name = FName("C1S4");
	InputEnabled = true;
	// uses pawn camera
	CamTarget = nullptr;
	UsePawnCam = true;
	UseFadeTime = false;
	
	// wait for user to solve the puzzle
	FlagsFinish = { "Inter.Trigger.PuzzleI01" };
}
