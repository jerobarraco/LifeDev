// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S000.h"

#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC1S000::ALStepC1S000():Super() {
	Name = FName("C1S0");
	static FText ST = FText::FromString("~ Whatever Works ~");
	Title = ST;
	
	DlgId = FName("C1S0"); // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = true;
	UseFadeTime = true;
	UseFade = true;
	FinishPostWait = false; // will be set by seqid anyway
	CamTarget = nullptr; // use previous camera
	Music = FSoftObjectPath("/Game/LifeDev/Game/Sounds/Music/Music02/Music02_MS.Music02_MS");
	// TODO set the load/unload here
}

void ALStepC1S000::Start_Implementation() {
	Super::Start_Implementation();
	// do always, not debug, since we could be skipping the chapter 0
	if (!Inventory->Has(LDConsts::Items::Card0)) {
		Inventory->Mod(LDConsts::Items::Card0, 1);
	}
	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	Flashback->SetMax(.75);
	Flashback->SetMin(0);
	Flashback->SetVal(0, .5);
}


void ALStepC1S000::Debug_Implementation() {
	Super::Debug_Implementation();
	Inventory->Mod("WM", 1);
}
