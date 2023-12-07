// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC1S000::ALStepC1S000():Super() {
	Name = FName("C1S0");
	static FText ST = FText::FromString("~ Whatever Works ~");
	Title = ST;
	
	DlgId = FName("C1S0"); // this chapter finishes after the dialog
	InputEnabled = false;
	TeleportChar = true; // move the char to where this camera is
	CamTarget = nullptr; // use pawn cam
	UsePawnCam = true;
	UseFadeTime = true;
	UseFade = true;
	FinishPostWait = false; // will be set by seqid anyway
	Music = FSoftObjectPath("/Game/LifeDev/Game/Var/Music/Music02/Music02_MS.Music02_MS");

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Chaps/All/DataLayers/Chap01_DL.Chap01_DL"));
	if (CDL1.Succeeded()) {
		DL_Load.Add(CDL1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL2 (TEXT("/Game/LifeDev/Game/Chaps/All/DataLayers/Chap00_DL.Chap00_DL"));
	if (CDL2.Succeeded()) {
		DL_Unload.Add(CDL2.Object);
	}
}

void ALStepC1S000::Start_Implementation() {
	Super::Start_Implementation();
	// do always, not debug, since we could be skipping the chapter 0
	if (!Inventory->Has(LDConsts::Items::Card0)) {
		Inventory->Mod(LDConsts::Items::Card0, 1);
	}

	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (!Flashback) return;
	Flashback->SetMax(.75);
	Flashback->SetMin(0);
	Flashback->SetVal(0, .5);
}


void ALStepC1S000::Debug_Implementation() {
	Super::Debug_Implementation();
	Inventory->Mod("WM", 1);
}
