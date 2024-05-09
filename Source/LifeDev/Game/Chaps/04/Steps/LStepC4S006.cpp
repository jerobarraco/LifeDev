// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S006.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S006::ALStepC4S006():Super() {
	Name = FName("C4S6");
	UseFade = false;
	UseRain = true;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S006::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetVal(.2, 180);
}

// enables the sponge (set in editor)
