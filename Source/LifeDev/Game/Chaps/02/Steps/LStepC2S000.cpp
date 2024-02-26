// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S000.h"

#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"

ALStepC2S000::ALStepC2S000():Super() {
	Name = FName("C2S0");
	static FText ST = FText::FromString("~ Yin ~");
	Title = ST;
	UseFade = true;
	// DlgId = FName("C2S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	UseFadeTime = true;
	FinishPostWait = false;
	TeleportChar = true;
	Music = FSoftObjectPath("/Game/LifeDev/Game/Env/Music/Music05/Music05_MS.Music05_MS");
	ItemsEnsure = {
		LDConsts::Items::Walkman,
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
	};

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap02_DL.Chap02_DL"));
	if (CDL1.Succeeded()) DL_Load.Add(CDL1.Object);
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO3 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C"));
	DL_Load.AddUnique(CDLO3.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap01_DL.Chap01_DL"));
	if (CDL2.Succeeded()) DL_Unload.Add(CDL2.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A"));
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_B.Outside_B"));
	DL_Unload.AddUnique(CDLO1.Object);
	DL_Unload.AddUnique(CDLO2.Object);
}

void ALStepC2S000::Start_Implementation() {
	Super::Start_Implementation();

	if (!FB) return;
	FB->SetMax(.6);
	FB->SetMin(0);
	FB->SetVal(.11, 10);
}

void ALStepC2S000::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	Inventory->Ensure(LDConsts::Items::Batts);
	Inventory->Ensure(LDConsts::Items::Card2);
	Inventory->Ensure(LDConsts::Items::Poem2);
}
