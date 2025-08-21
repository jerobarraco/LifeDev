// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC3S002.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC3S002::ALStepC3S002():Super() {
	Label = FName("C3S2");
	InputEnabled = true;
	UsePawnCam = true;
	// will decrease the fb. done in this step since this step could potentially last a while until they find the
	// required items, and it's hard to find them with a fb
	RandFB->DelayMin = .5;
	RandFB->DelayMax = 2;
	RandFB->ValueMin = -.05; // i've lowered from .07 since we have the card now
	RandFB->ValueMax = .02; // you wouldn't think is so easy to get out of a flashback, do you?
	UseFBRand = true;
	UseRain = true;
	FinishItems = {
		LDConsts::Items::Card3,
		LDConsts::Items::Poem3,
		LDConsts::Items::Tape3,
	};
	RemoveItems = {LDConsts::Items::Tape3, LDConsts::Items::Batts}; // consume
	IntersHint = { // card already has hint. hint the book (poem has hint), hint the pictures (tape has hint, so does batts, pictures hints the plant)
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.BooksI00_UAID_D8BBC116E5014AD901_1989999335")),
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI06_UAID_D8BBC116E5014A2C02_1407893984")),
	};
}

void ALStepC3S002::BeginPlay() {
	Super::BeginPlay();

	// force fade the card
	DoIntersFade(IntersFadeIn, false);
}

void ALStepC3S002::TryStart_Implementation() {
	Super::TryStart_Implementation();
	FB->SetMin(0);
}

// IntersFade is set to the card