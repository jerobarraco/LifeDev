// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S000.h"

#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"

ALStepC3S000::ALStepC3S000():Super() {
	Name = FName("C3S0");
	static FText ST = FText::FromString("~ Perseverance ~");
	Title = ST;
	UseFade = true;
	// DlgId = FName("C3S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	UseFadeTime = true;
	FinishPostWait = false;
	TeleportChar = true;

	// TODO temporary until i get a new music
	Music = FSoftObjectPath("/Game/LifeDev/Game/Env/Music/Music01/Music01_MS.Music01_MS");

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap03_DL.Chap03_DL"));
	if (CDL1.Succeeded()) DL_Load.Add(CDL1.Object);
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap02_DL.Chap02_DL"));
	if (CDL2.Succeeded()) DL_Unload.Add(CDL2.Object);

	ItemsEnsure = {
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
		LDConsts::Items::Card2,
		LDConsts::Items::Walkman
	};
}

void ALStepC3S000::TryStart_Implementation() {
	Super::TryStart_Implementation();
	FB->SetMax(.4);
	FB->SetVal(.01);
}

void ALStepC3S000::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	if (Inventory) Inventory->Ensure(LDConsts::Items::Card3);
}
