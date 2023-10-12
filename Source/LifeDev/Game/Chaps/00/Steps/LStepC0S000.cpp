// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"

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
	Music = FSoftObjectPath("/Game/LifeDev/Game/Sounds/Music/Music04/Music04_MS.Music04_MS");
	RandFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RandFlashback"));
	// No need to load, it should be loaded by default?
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Chaps/All/DataLayers/Chap00_DL.Chap00_DL"));
	if (CDL1.Succeeded()) {
		DL_Load.Add(CDL1.Object);
	}
}

void ALStepC0S000::ShowDoors(bool Chap00) {
	// TODO remove this once the data-layer-loading is implemented
	if (IsValid(C0Door)) {
		C0Door->SetActorHiddenInGame(!Chap00);
	}
	if(IsValid(C1Door)) {
		C1Door->SetActorHiddenInGame(Chap00);
	}
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();

	UWorld* const W = GetWorld();
	
	ALMusicMan::SetEnvironS(W, true);

	UFlashback* const Flashback = UFlashback::Get(W);
	if (!Flashback) {
		UE_LOG(LogTemp, Warning, TEXT("ALStepC0S000 Start: can't get the flashback subsystem."));
		return;
	}

	Flashback->SetVal(.1);
	Flashback->SetMax(.75);
	RandFB->Activate(true);
	ShowDoors(true);
}

void ALStepC0S000::Stop_Implementation() {
	RandFB->Deactivate();
	ShowDoors(false);
	Super::Stop_Implementation();
	// UFlashback::SetValS(GetWorld(), 0, 5);
}
