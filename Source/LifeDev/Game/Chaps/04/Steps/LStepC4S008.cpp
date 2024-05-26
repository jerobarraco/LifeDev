// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S008.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S008::ALStepC4S008():Super() {
	Name = FName("C4S8");
	UseFade = false;
	UseRain = true;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;

	UseRandFB = true; // min is limited in c4s0
	RandFB->DelayMin = .2;
	RandFB->DelayMax = .75;
	RandFB->ValueMin = -.015;
	RandFB->ValueMax = .005;
}

// allows the player to sit on the table. enables a spot (set on editor)
