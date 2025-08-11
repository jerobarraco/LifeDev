// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC4S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC4S000::ALStepC4S000():Super() {
	Name = FName("C4S0");
	const static FText ST = NSLOCTEXT("C4S0", "Title", "~ Acceptance ~");
	Title = ST;
	UseFade = true;
	// DlgId = FName("C4S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	CamBlendTime = 0;
	TeleportChar = true;
	FinishItems = {LDConsts::Items::Card4};
	EnsureItems = {
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
		LDConsts::Items::Card2,
		LDConsts::Items::Card3,
		LDConsts::Items::Walkman
	};

	Music = FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music07/Music06-07_MS.Music06-07_MS"));
	// needed
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();

	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap04_DL.Chap04_DL")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Kitchen.Kitchen")));

	// unload
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap03_DL.Chap03_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall00.Hall00")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Living00.Living00")));
	// unload unnecessary outside props
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_D.Outside_D")));
}

void ALStepC4S000::TryStart_Implementation() {
	Super::TryStart_Implementation();

	FB->SetMax(1);
	FB->SetMin(.2); // this limits the fbs on the next steps too.
	FB->SetVal(.2, .15);
}
