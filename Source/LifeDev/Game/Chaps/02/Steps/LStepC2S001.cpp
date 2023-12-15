// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S001.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC2S001::ALStepC2S001():Super() {
	Name = FName("C2S1");
	UseFade = false;
	// DlgId = FName("C2S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = false;
	UseFadeTime = false;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC2S001::Start_Implementation() {
	Super::Start_Implementation();
	
	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (!Flashback) return;
	Flashback->SetMax(.75);
	Flashback->SetMin(0);
	Flashback->SetVal(.5);
}
