// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S008.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S008::ALStepC4S008():Super() {
	Name = FName("C4S8");
	UseFade = false;
	UseRain = true;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	FinishPostWait = false;
	UsePawnCam = true;
	TeleportChar = false;
	FinishFlags = {"Inter.Trigger.SpotI03"};
	UseFBRand = true; // min is limited in c4s0
	RandFB->DelayMin = .5;
	RandFB->DelayMax = .85;
	RandFB->ValueMin = -.010;
	RandFB->ValueMax = .005;
}

// allows the player to sit on the table. enables a spot (set on editor)
