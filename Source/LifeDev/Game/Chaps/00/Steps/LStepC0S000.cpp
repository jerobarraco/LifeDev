// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	// DlgId = FName("Intro");
	InputEnabled = true;
	UseFadeTime = true;
	FinishPostWait = false;
	UsePawnCam = true;
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();

	UFlashback::SetValS(GetWorld(), 0.1);
}

void ALStepC0S000::Stop_Implementation() {
	Super::Stop_Implementation();
	
	// UFlashback::SetValS(GetWorld(), 0, 5);
}

void ALStepC0S000::BeginPlay() {
	Super::BeginPlay();
}
