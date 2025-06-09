// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC1S000::ALStepC1S000():Super() {
	Name = FName("C1S0");
	const static FText ST = NSLOCTEXT("C1S0", "Title", "~ Whatever Works ~");
	Title = ST;
	
	DlgId = FName("C1S0"); // this chapter finishes after the dialog
	InputEnabled = false;
	TeleportChar = true; // move the char to where this camera is
	CamTarget = nullptr; // use pawn cam
	UsePawnCam = true;
	UseFade = true;
	FinishPostWait = false; // will be set by seqid anyway

	Music = FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music02/Music02_MS.Music02_MS"));
	// this one, seems to be loaded well without the patch :/? HOW?!
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous(); // left just in case
	
	EnsureItems = {
		// do always, not debug, since we could be skipping the chapter 0
		LDConsts::Items::Card0,
		LDConsts::Items::Poem0
	};

	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap01_DL.Chap01_DL")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room00.Room00")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room01.Room01")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_B.Outside_B")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C")));

	// unload
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap00_DL.Chap00_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02.Room02")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room03.Room03")));
}

void ALStepC1S000::TryStart_Implementation() {
	Super::TryStart_Implementation();

	if (UNLIKELY(!FB)) return;
	// important to set a short duration since this comes from another chapter
	// that happened some time ago.
	FB->SetMin(0, .1);
	FB->SetMax(.6, .1);
	FB->SetVal(0, .15); // might not have an effect if setmin or setmax change the val
}

void ALStepC1S000::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	Inventory->Ensure("WM");
}
