// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S000.h"

#include "Inventory/Inventory.h"

ALStepC1S000::ALStepC1S000():Super() {
	Name = FName("C1S0");
	DlgId = FName("C1S0"); // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = true;
	UseFadeTime = true;
	FinishPostWait = false; // will be set by seqid anyway
	CamTarget = nullptr; // use previous camera
}

void ALStepC1S000::Start_Implementation() {
	Super::Start_Implementation();
	UInventory* Inventory = GetWorld()->GetSubsystem<UInventory>();
    if (!IsValid(Inventory)) return;
	Inventory->Mod("C0", 1);
}

void ALStepC1S000::Debug_Implementation() {
	Super::Debug_Implementation();
	UInventory* Inventory = GetWorld()->GetSubsystem<UInventory>();
	if (!IsValid(Inventory)) return;
	Inventory->Mod("Walkman", 1);
	Inventory->Mod("C1KD1", 1);
}
