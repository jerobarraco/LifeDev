// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S002.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S002::ALStepC4S002():Super() {
	Name = FName("C4S2");
	// DlgId = FName("C4S1");
	UseFade = false;
	UseRain = true;
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
	UseRandFB = true;

	RandFB->DelayMin = .2;
	RandFB->DelayMax = .75;
	RandFB->ValueMin = -.02;
	RandFB->ValueMax = .005;
	// min is limited in c4s0
}

// enables the pot
