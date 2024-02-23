// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S001.h"

#include "Camera/CameraComponent.h"
#include "LifeDev/Game/Flashback/Flashback.h"

#include "LifeDev/Game/Chaps/All/NPCs/LNPC03.h"

ALStepC3S001::ALStepC3S001():Super() {
	Name = FName("C3S1");
	UseFade = false;
	DlgId = FName("C3S1"); // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;
	
	Cam->SetConstraintAspectRatio(true);
	Cam->AspectRatio = 2;

	GhostPos = FVector(200,-42,65);
	UseGhosts = true;
}

void ALStepC3S001::Start_Implementation() {
	UWorld* const W = GetWorld();
	if (!W) return;

	// make the fb raise progressively with the dialogs
	FB->SetMax(1);

	constexpr int32 numDlgs = 4; //TODO??
	FbDiagMod = (1.0-FB->GetValTo()) / (numDlgs-1);
	
	Super::Start_Implementation();

	ALNPC03* const NPC = Cast<ALNPC03>(Actor);
	if (NPC) NPC->SetScold();
}

void ALStepC3S001::Stop_Implementation() {
	if (FB) {
		// FB->SetVal(.05, 10);
		FB->SetMin(.33,1); // TODO
	}
	Super::Stop_Implementation();
}
