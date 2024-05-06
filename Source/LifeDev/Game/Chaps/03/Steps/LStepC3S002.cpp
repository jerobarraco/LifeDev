// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S002.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC3S002::ALStepC3S002():Super() {
	Name = FName("C3S2");
	UseFade = false;
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;

	ItemsFinish = {
		LDConsts::Items::Card3,
		LDConsts::Items::Poem3,
	};
	
	// will decrease the fb. done in this step since this step could potentially last a while until they find the
	// required items, and it's hard to find them with a fb
	RndFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RndFB"));
	RndFB->SetAutoActivate(false);
	RndFB->DelayMin = .5;
	RndFB->DelayMax = 2;
	RndFB->ValueMin = -.07;
	RndFB->ValueMax = .02; // you wouldn't think is so easy to get out of a flashback, do you?
}

void ALStepC3S002::BeginPlay() {
	Super::BeginPlay();
	if (IntersFadeIn.Num()<1) {
		UE_LOG(LogTemp, Warning, TEXT("C3S2: Card not set on intersfade"));
		return;
	}

	// force fade the card
	DoIntersFade(IntersFadeIn, false);
}

void ALStepC3S002::TryStart_Implementation() {
	Super::TryStart_Implementation();
	ALMusicMan::SetRainS(GetWorld(), true);
	FB->SetMin(0);
	RndFB->Activate(true);
}

void ALStepC3S002::Stop_Implementation() {
	RndFB->Deactivate();
	FB->SetMin(0);
	FB->SetMax(1);
	FB->SetVal(0);
	ALMusicMan::SetRainS(GetWorld(), false);
	Super::Stop_Implementation();
}

// IntersFade is set to the card