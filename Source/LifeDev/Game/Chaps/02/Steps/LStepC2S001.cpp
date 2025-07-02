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
	TeleportChar = true; // teleport after the blend

	UseGhosts = true;
	GhostPos = FVector(210,-42,-65);

	FBDlgAutoTo = 1.2;
	UseFBDlgAuto = true;
	UseFBAnim = true;

	ActorsShow = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC02B_B_C_UAID_D8BBC116E501CCC501_1291009115"))
	};
	ActorsHide = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC02B_B_C_UAID_D8BBC116E501CCC501_1291009115"))
	};
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

