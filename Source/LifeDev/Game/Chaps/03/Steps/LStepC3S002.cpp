// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC3S002.h"

#include "Kismet/GameplayStatics.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Game/Chaps/All/Inters/Cards/Card03.h"

ALStepC3S002::ALStepC3S002():Super() {
	Name = FName("C3S2");
	UseFade = false;
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;
	ItemsFinish = {
		LDConsts::Items::Card3,
		LDConsts::Items::Poem3,
		LDConsts::Items::Tape3,
	};
	ItemsRem = {LDConsts::Items::Tape3, LDConsts::Items::Batts}; // consume
	// will decrease the fb. done in this step since this step could potentially last a while until they find the
	// required items, and it's hard to find them with a fb
	RandFB->DelayMin = .5;
	RandFB->DelayMax = 2;
	RandFB->ValueMin = -.07;
	RandFB->ValueMax = .02; // you wouldn't think is so easy to get out of a flashback, do you?
	UseFBRand = true;
	UseRain = true;
}

void ALStepC3S002::BeginPlay() {
	Super::BeginPlay();
	if (UNLIKELY(IntersFadeIn.Num()<1)) {
		UE_LOG(LogTemp, Warning, TEXT("C3S2: Card not set on intersfade"));
		IntersFadeIn.AddUnique(
			Cast<ALInteract>(
				UGameplayStatics::GetActorOfClass(this, ACard03::StaticClass())));
	}

	// force fade the card
	DoIntersFade(IntersFadeIn, false);
}

void ALStepC3S002::TryStart_Implementation() {
	Super::TryStart_Implementation();
	FB->SetMin(0);
}

// IntersFade is set to the card