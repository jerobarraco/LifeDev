// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S002.h"

#include "LifeDev/Game/Interact/LInteract.h"

ALStepC4S002::ALStepC4S002():Super() {
	Name = FName("C4S2");
	UseFade = false;
	// DlgId = FName("C4S1"); 
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S002::DoStart_Implementation() {
	Super::DoStart_Implementation();
	if (Pot) Pot->SetEnabled(true);
}
