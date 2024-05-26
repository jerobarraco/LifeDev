// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Interact/Animator/CRandomizer.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstFlags.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	
	static FText STitle = FText::FromString("~ Intention ~");
	Title = STitle;
	
	// DlgId = FName("Intro");
	InputEnabled = true;
	UseFade = true;
	UseFadeTime = true;
	FinishPostWait = false;
	UsePawnCam = true;
	UseRandFB = true;
	Music = FSoftObjectPath("/Game/LifeDev/Game/Env/Music/Music04/Music04_MS.Music04_MS");
	// No need to load, it should be loaded by default?
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap00_DL.Chap00_DL"));
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLOutA(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A"));
	if (CDL1.Succeeded()) DL_Load.Add(CDL1.Object);
	if (CDLOutA.Succeeded()) DL_Load.Add(CDLOutA.Object);
}

void ALStepC0S000::TryStart_Implementation() {
	Super::TryStart_Implementation();

	if (!FB) {
		UE_LOG(LogTemp, Warning, TEXT("ALStepC0S000 Start: can't get the FB subsystem."));
		return;
	}

	FB->SetVal(.1);
	FB->SetMax(.75);
}

void ALStepC0S000::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	Inventory->Ensure(LDConsts::Items::Poem0);
	Inventory->Ensure(LDConsts::Items::Card0);
	Flags->Set(LDConsts::Flags::CH0::Basin);
	Flags->Set(LDConsts::Flags::CH0::Spot);
}
