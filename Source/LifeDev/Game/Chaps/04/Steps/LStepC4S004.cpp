// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S004.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S004::ALStepC4S004():Super() {
	Name = FName("C4S4");
	// DlgId = FName("C4S1"); 
	UseFade = false;
	UseRain = true;
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

// enables the fridge and rice and pot
