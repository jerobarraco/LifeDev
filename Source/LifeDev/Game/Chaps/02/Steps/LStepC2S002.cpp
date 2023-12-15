// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S002.h"

ALStepC2S002::ALStepC2S002():Super() {
	static const FName N("C2S2");
	Name = N;
	// DlgId = N; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = true;
	UsePawnCam = true;
	UseFadeTime = false;
	FinishPostWait = false;
	TeleportChar = true;
}

void ALStepC2S002::BeginPlay() {
	Super::BeginPlay();
}

void ALStepC2S002::Start_Implementation() {
	Super::Start_Implementation();
}

void ALStepC2S002::Stop_Implementation() {
	Super::Stop_Implementation();
}
