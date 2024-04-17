// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S003.h"

#include "LifeDev/Game/Interact/LInteract.h"

ALStepC4S003::ALStepC4S003():Super() {
	Name = FName("C4S3");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S003::Stop_Implementation() {
	if (DisableChar) DisableChar->Fade(false);
	Super::Stop_Implementation();
}
