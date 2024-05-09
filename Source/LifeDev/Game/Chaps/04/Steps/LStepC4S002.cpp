// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S002.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S002::ALStepC4S002():Super() {
	Name = FName("C4S2");
	// DlgId = FName("C4S1");
	UseFade = false;
	UseRain = true;
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S002::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetVal(.2, 180);
}

// enables the pot
