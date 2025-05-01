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
	FlagsFinish = {"Inter.Trigger.PuzzleI04"};
	// music needs the .Music08_MS postfix
	Music = FSoftObjectPath("/Game/LifeDev/Game/Env/Music/Music08/Music08_MS.Music08_MS");
	// needed
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap03_DL.Chap03_DL"));
	if (LIKELY(CDL1.Succeeded())) DL_Load.Add(CDL1.Object);

	// ensure to load these two. even though they are loaded by a previous chapter,
	// the player could jump straight to this chapter via a savegame (or hack).
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO3 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C"));
	if (LIKELY(CDLO3.Succeeded())) DL_Load.AddUnique(CDLO3.Object);

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLO4 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_D.Outside_D"));
	if (LIKELY(CDLO4.Succeeded())) DL_Load.Add(CDLO4.Object);
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL3 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap02_DL.Chap02_DL"));
	if (LIKELY(CDL3.Succeeded())) DL_Unload.Add(CDL3.Object);
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLR2 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02"));
	if (LIKELY(CDLR2.Succeeded())) DL_Unload.Add(CDLR2.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLH0 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall00"));
	if (LIKELY(CDLH0.Succeeded())) DL_Unload.Add(CDLH0.Object);

	ItemsEnsure = {
		LDConsts::Items::Card0,
		LDConsts::Items::Card1,
		LDConsts::Items::Card2,
		LDConsts::Items::Walkman
	};
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
