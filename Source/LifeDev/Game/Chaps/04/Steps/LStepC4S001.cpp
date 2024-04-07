// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S001.h"

#include "LifeDev/Game/Interact/LInteract.h"

ALStepC4S001::ALStepC4S001():Super() {
	Name = FName("C4S1");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	WaitTime = CamBlendTime = 2;
	// set wait to blend so that FIRST you turn and THEN it fades the chars (on DoStart (after wait))
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S001::BeginPlay() {
	Super::BeginPlay();

	for (ALInteract* const I: Chars) {
		I->Fade(false);
	}
}

void ALStepC4S001::DoStart_Implementation() {
	Super::DoStart_Implementation();
	// fade on start to use
	for (uint8 i = 0; i< Chars.Num(); ++i) {
		ALInteract* const I = Chars[i];
		if (!I) continue;
        I->Fade(true);
		// no talking on the table (actually makes it difficult because of the steps so...)
		I->SetEnabled(false); // I->SetEnabled(i==0);
    }
}
