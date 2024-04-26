// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S007.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S007::ALStepC4S007():Super() {
	Name = FName("C4S7");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;
	TeleportChar = false;
	FbDiagMod = .1;
}

void ALStepC4S007::Stop_Implementation() {
	FB->SetVal(.05);
	Super::Stop_Implementation();
}

// interfadeout on editor
