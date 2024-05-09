// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S008.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S008::ALStepC4S008():Super() {
	Name = FName("C4S8");
	UseFade = false;
	UseRain = true;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S008::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetVal(.2, 180);
}

// allows the player to sit on the table. enables a spot (set on editor)
