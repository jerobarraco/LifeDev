// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S000.h"

#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"

ALStepC3S000::ALStepC3S000():Super() {
	Name = SName;
	const static FText ST = NSLOCTEXT("C3S0", "Title", "~ Perseverance ~");
	Title = ST;
	UseFade = true;
	// DlgId = FName("C3S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	UseFadeTime = true;
	FinishPostWait = false;
	TeleportChar = true;
	FinishFlags = {"Inter.Trigger.PuzzleI04"};
	EnsureItems = {
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
		LDConsts::Items::Card2,
		LDConsts::Items::Walkman
	};

	// this is ok, even though the step is always loaded, since the dls are loaded before start is triggered.
	IntersActivate = {
		// the picture puzzle
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI06_UAID_D8BBC116E5014A2C02_1407893984"))
	};
	// music needs the .Music08_MS postfix
	Music = FSoftObjectPath("/Game/LifeDev/Game/Env/Music/Music08/Music08_MS.Music08_MS");
	// needed
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();

	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap03_DL.Chap03_DL")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Living00.Living00")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall00.Hall00")));
	// ensure to load these two. even though they are loaded by a previous chapter,
	// the player could jump straight to this chapter via a savegame (or hack).
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_D.Outside_D")));

	// unload
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap02_DL.Chap02_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02.Room02")));
}

void ALStepC3S000::TryStart_Implementation() {
	Super::TryStart_Implementation();
	
	FB->SetMin(0);
	FB->SetMax(.4);
	FB->SetVal(.01, .15);
}

void ALStepC3S000::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	if (LIKELY(Inventory)) Inventory->Ensure(LDConsts::Items::Card3);
}
