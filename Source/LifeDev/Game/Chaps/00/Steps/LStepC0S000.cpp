// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Interact/Animator/CRandomizer.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"
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
	Music = FSoftObjectPath("/Game/LifeDev/Game/Var/Music/Music04/Music04_MS.Music04_MS");
	RandFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RandFlashback"));
	// No need to load, it should be loaded by default?
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Var/Datalayers/Chap00_DL.Chap00_DL"));
	if (CDL1.Succeeded()) {
		DL_Load.Add(CDL1.Object);
	}
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();

	UWorld* const W = GetWorld();
	if (!W) return;
	ALMusicMan::SetEnvironS(W, true);

	if (!FB) {
		UE_LOG(LogTemp, Warning, TEXT("ALStepC0S000 Start: can't get the FB subsystem."));
		return;
	}

	FB->SetVal(.1);
	FB->SetMax(.75);
	RandFB->Activate(true);
}

void ALStepC0S000::Stop_Implementation() {
	RandFB->Deactivate();
	Super::Stop_Implementation();
	// UFlashback::SetValS(GetWorld(), 0, 5);
}

void ALStepC0S000::Debug_Implementation() {
	Super::Debug_Implementation();
	Inventory->Mod(LDConsts::Items::Poem0, 1);
}
