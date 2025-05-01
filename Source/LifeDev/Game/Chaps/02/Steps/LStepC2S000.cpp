// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC2S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Inventory/Inventory.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC2S000::ALStepC2S000():Super() {
	Name = FName("C2S0");
	const static FText ST = NSLOCTEXT("C2S0", "Title", "~ Yin ~");
	Title = ST;
	UseFade = true;
	UseFadeTime = true;
	FinishPostWait = false;
	// DlgId = FName("C2S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	TeleportChar = true;
	Music = FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music05/Music05_MS.Music05_MS"));
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();

	ItemsEnsure = {
		LDConsts::Items::Walkman,
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
	};

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap02_DL.Chap02_DL"));
	if (LIKELY(CDL1.Succeeded())) DL_Load.Add(CDL1.Object);
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO3 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C"));
	if (LIKELY(CDLO3.Succeeded())) DL_Load.AddUnique(CDLO3.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO4 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_D.Outside_D"));
	if (LIKELY(CDLO4.Succeeded())) DL_Load.Add(CDLO4.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLR2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02"));
	if (LIKELY(CDLR2.Succeeded())) DL_Load.Add(CDLR2.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLH0 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall00"));
	if (LIKELY(CDLH0.Succeeded())) DL_Load.Add(CDLH0.Object);

	// unload
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap01_DL.Chap01_DL"));
	if (LIKELY(CDL2.Succeeded())) DL_Unload.Add(CDL2.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLR0(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room00"));
	if (LIKELY(CDLR0.Succeeded())) DL_Unload.Add(CDLR0.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLR1(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room01"));
	if (LIKELY(CDLR1.Succeeded())) DL_Unload.Add(CDLR1.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A"));
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_B.Outside_B"));
	DL_Unload.AddUnique(CDLO1.Object);
	DL_Unload.AddUnique(CDLO2.Object);
}

void ALStepC2S000::TryStart_Implementation() {
	Super::TryStart_Implementation();

	if (UNLIKELY(!FB)) return;
	FB->SetMax(.6);
	FB->SetMin(0);
	FB->SetVal(.11, .15);
}

void ALStepC2S000::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	Inventory->Ensure(LDConsts::Items::Batts);
	Inventory->Ensure(LDConsts::Items::Card2);
	Inventory->Ensure(LDConsts::Items::Poem2);
}
