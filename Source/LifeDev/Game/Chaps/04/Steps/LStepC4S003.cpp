// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S003.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S003::ALStepC4S003():Super() {
	Name = FName("C4S3");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;
	TeleportChar = false;
	// go a bit above to ensure we end up at 1 a couple of dialogs before
	FBDiagAutoTo = 1.2;
	UseFBDiagAuto = true;
}

void ALStepC4S003::Stop_Implementation() {
	FB->SetVal(.2);
	Super::Stop_Implementation();
}

// interfadeout in editor

