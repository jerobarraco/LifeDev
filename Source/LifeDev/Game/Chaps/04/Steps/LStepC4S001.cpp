// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S001.h"

#include "LifeDev/Game/Interact/LInteract.h"

ALStepC4S001::ALStepC4S001():Super() {
	Name = FName("C4S1");
	UseFade = false;
	// DlgId = FName("C4S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S001::BeginPlay() {
	Super::BeginPlay();

	for (ALInteract* const I: Chars) {
		I->Fade(false);
	}
}

void ALStepC4S001::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
}

void ALStepC4S001::DoStart_Implementation() {
	Super::DoStart_Implementation();
	// fade on start to use 
    for (ALInteract* const I: Chars) {
        I->Fade(true);
    }
}
