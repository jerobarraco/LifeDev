// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S009.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S009::ALStepC4S009():Super() {
	Name = FName("C4S9");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;
	TeleportChar = false;
	WaitTime = CamBlendTime = 2; // force dialog to start after blend
	UseFBDiagAuto=true;
	FBDiagAutoTo = -.1;
}

void ALStepC4S009::BeginPlay() {
	Super::BeginPlay();
	DoIntersFade(IntersFadeIn, false); // forcefully hide the ones we're showing.
}

void ALStepC4S009::Start_Implementation() {
	Super::Start_Implementation();
	// for this we want to really reach 0 since it's the very end (for dramatic purposes, not realism). 
	if (FB) FB->SetMin(0);
}
