// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	// DlgId = FName("Intro");
	InputEnabled = true;
	UseFadeTime = true;
	FinishPostWait = false;
	UsePawnCam = true;

	RandFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RandFlashback"));
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();

	UFlashback* Flashback = UFlashback::Get(GetWorld());
	if (!Flashback) {
		UE_LOG(LogTemp, Warning, TEXT("ALStepC0S000 Start: can't get the flashback subsystem."));
		return;
	}

	Flashback->SetVal(.1);
	Flashback->SetMax(.75);
	RandFB->Activate(true);
}

void ALStepC0S000::Stop_Implementation() {
	RandFB->Deactivate();
	Super::Stop_Implementation();
	// UFlashback::SetValS(GetWorld(), 0, 5);
}
