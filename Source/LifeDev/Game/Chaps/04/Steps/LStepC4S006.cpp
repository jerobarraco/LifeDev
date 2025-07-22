// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S006.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S006::ALStepC4S006():Super() {
	Name = FName("C4S6");
	UseRain = true;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	FinishPostWait = false;
	UsePawnCam = true;
	TeleportChar = false;
	FinishFlags = {"Inter.Trigger.SpongeI00"};
	UseFBRand = true; // min is limited in c4s0
	RandFB->DelayMin = .5;
	RandFB->DelayMax = .85;
	RandFB->ValueMin = -.015;
	RandFB->ValueMax = .005;
}

// enables the sponge (set in editor)
