// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC3S001.h"

#include "LifeDev/Game/Flashback/Flashback.h"

#include "LifeDev/Game/Chaps/All/NPCs/LNPC03.h"

ALStepC3S001::ALStepC3S001():Super() {
	Name = FName("C3S1");
	DlgId = FName("C3S1"); // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	TeleportChar = true;

	GhostPos = FVector(200,-42,65);
	UseGhosts = true;
	UseFBAnim = true;
	UseFBDlgAuto = true;
	FBDlgAutoTo = 1.2;
	ActorsShow = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC03_UAID_D8BBC116E501DDDA01_2038493305"))
	};
	
	ActorsHide = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC03_UAID_D8BBC116E501DDDA01_2038493305"))
	};
}

void ALStepC3S001::TryStart_Implementation() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	// make the fb raise progressively with the dialogs
	FB->SetMax(1);

	// calculate the correct fbdiagmod before calling TryStart
	// constexpr int32 numDlgs = 4;
	// FBDiagMod = (1.0-FB->GetValTo()) / (numDlgs-1);

	Super::TryStart_Implementation();

	if (ActorsShow.Num() < 1) {
		UE_LOG(LogTemp, Warning, TEXT("NPC03 not set in c3s1."));
		return;
	}

	ALNPC03* const NPC = Cast<ALNPC03>(ActorsShow[0].Get());
	if (LIKELY(NPC)) NPC->SetPoseScold();
}

// actorsShow set in editor