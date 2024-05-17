// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S004.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S004::ALStepC4S004():Super() {
	Name = FName("C4S4");
	// DlgId = FName("C4S1"); 
	UseFade = false;
	UseRain = true;
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S004::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetVal(.2, 60);
}

// enables the fridge and rice and pot
