// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC2S001.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC2S001::ALStepC2S001():Super() {
	static const FName N("C2S1");
	Name = N;
	DlgId = N; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;
	TeleportChar = false;

	UseGhosts = true;
	GhostPos = FVector(210,-42,-65);
	// there are 11 dialogs, and the fb should be on .4 at least.
	// with .08 we ensure that we reach 1. but with .1 is too much
	// .075 a couple of diags remain
	// FBDiagMod = .07;
	FBDlgAutoTo = 1.2;
	UseFBDlgAuto = true;
	UseFBAnim = true;
}

void ALStepC2S001::TryStart_Implementation() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	// make the fb raise progressively with the dialogs
	FB->SetMax(1);
	
	Super::TryStart_Implementation();
}

void ALStepC2S001::Stop_Implementation() {
	if (LIKELY(FB)) {
		// FB->SetVal(.05, 10);
		
		// clamp till the end of the story, important for the randomizer in c2s4
		// .33 is the min to hear the melody
		FB->SetMin(.33,1);
	}

	Super::Stop_Implementation();
}

