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
	UseFadeTime = true;
	FinishPostWait = false;
	TeleportChar = true;
	ItemsFinish = {LDConsts::Items::Card4};

	Music = FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music07/Music06-07_MS.Music06-07_MS"));
	// needed
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap04_DL.Chap04_DL"));
	if (LIKELY(CDL1.Succeeded())) DL_Load.Add(CDL1.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLK0 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Kitchen"));
	if (LIKELY(CDLK0.Succeeded())) DL_Load.Add(CDLK0.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap03_DL.Chap03_DL"));
	if (LIKELY(CDL2.Succeeded())) DL_Unload.Add(CDL2.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLH0 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall00"));
	if (LIKELY(CDLH0.Succeeded())) DL_Unload.Add(CDLH0.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLL0 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Living00"));
	if (LIKELY(CDLL0.Succeeded())) DL_Unload.Add(CDLL0.Object);

	// unload unnecessary outside props
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO3 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C"));
	if (LIKELY(CDLO3.Succeeded())) DL_Unload.AddUnique(CDLO3.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO4 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_D.Outside_D"));
	if (LIKELY(CDLO4.Succeeded())) DL_Unload.Add(CDLO4.Object);

	ItemsEnsure = {
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
		LDConsts::Items::Card2,
		LDConsts::Items::Card3,
		LDConsts::Items::Walkman
	};
}

void ALStepC4S000::TryStart_Implementation() {
	Super::TryStart_Implementation();

	FB->SetMax(1);
	FB->SetMin(.2); // this limits the fbs on the next steps too.
	FB->SetVal(.2, .15);
}
