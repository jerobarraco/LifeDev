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
	FinishPostWait = false;
	// DlgId = FName("C2S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	TeleportChar = true;
	Music = FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music05/Music05_MS.Music05_MS"));
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();

	EnsureItems = {
		LDConsts::Items::Walkman,
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
	};

	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap02_DL.Chap02_DL")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_D.Outside_D")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02.Room02")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall00.Hall00")));

	// unload
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap01_DL.Chap01_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room00.Room00")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room01.Room01")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_B.Outside_B")));

	IntersFadeIn = { // fade in so that increasing the step does not hide them
		// battery and poem
		TSoftObjectPtr<ALInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.BattsI00_UAID_D8BBC116E5012EC501_1469610310")),
		TSoftObjectPtr<ALInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PoemI02_UAID_D8BBC116E5012CC501_1287868959"))
	};
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
