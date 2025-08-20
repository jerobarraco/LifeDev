// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S004.h"

ALStepC1S004::ALStepC1S004():Super() {
	Label = FName("C1S4");
	InputEnabled = true;
	// uses pawn camera
	CamTarget = nullptr;
	UsePawnCam = true;

	// wait for user to solve the puzzle
	FinishFlags = { "Inter.Trigger.PuzzleI01" };
}
