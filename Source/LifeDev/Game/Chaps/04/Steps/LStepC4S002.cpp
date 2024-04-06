// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S002.h"

ALStepC4S002::ALStepC4S002():Super() {
	Name = FName("C4S2");
	UseFade = false;
	// DlgId = FName("C4S1"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S002::BeginPlay() {
	Super::BeginPlay();
}

void ALStepC4S002::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
}

void ALStepC4S002::DoStart_Implementation() {
	Super::DoStart_Implementation();
}
