// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S001.h"

#include "LifeDev/Game/Interact/LInteract.h"

ALStepC4S001::ALStepC4S001():Super() {
	Name = FName("C4S1");
	DlgId = Name; // this chapter finishes after the dialog
	// includes the "im hungry"
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	WaitTime = CamBlendTime = 2;
	// FBDiagMod = .8/22.0; // .1; // only .8 available, and more than 22 dialogs.
	FBDlgAutoTo = 1.2;
	UseFBDlgAuto = true;
	// set wait to blend so that
	// FIRST you turn, and THEN it fades the chars (on Start (after wait))
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC4S001::BeginPlay() {
	Super::BeginPlay();
	// start with the inters faded out. a cheeky cheat to avoid doing it on each npci.
	DoIntersFade(IntersFadeIn, false);
}

void ALStepC4S001::Start_Implementation() {
	Super::Start_Implementation();
	// talking actually makes it difficult because of the dialogs.
	// this step finishes as the dialog finishes. so deactivate all but npci6
	// Super::Start would have faded them, but fade will set active.
	for (uint8 i = 0; i< IntersFadeIn.Num(); ++i) {
		AInteract* const I = IntersFadeIn[i].Get();
		if (UNLIKELY(!I)) continue;

		I->SetActive(i==0); // npci6 continues the flow next step
	}
}

// interfadeout and interfadein on editor
