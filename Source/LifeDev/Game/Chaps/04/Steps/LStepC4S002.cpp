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
	FlagsFinish = { "Inter.Trigger.StoveI00" };

	UseFBRand = true; // min is limited in c4s0
	RandFB->DelayMin = .5;
	RandFB->DelayMax = .85;
	RandFB->ValueMin = -.015;
	RandFB->ValueMax = .005;
}

