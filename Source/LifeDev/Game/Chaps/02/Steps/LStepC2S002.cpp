// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S002.h"

#include "Kismet/GameplayStatics.h"

ALStepC2S002::ALStepC2S002():Super() {
	static const FName N("C2S2");
	Name = N;
	// DlgId = N; // this chapter finishes after the dialog
	UseFade = false;
	// returns to the player camera and control
	InputEnabled = true;
	UsePawnCam = true;
	// teleport after the boss interaction happens
	TeleportChar = false;
	// no waits
	UseFadeTime = false;
	FinishPostWait = false;
	// wait for the tape 
	FinishItems = { "T02" };
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/Analog/Analog_C.Analog_C"));
	SFX_Analog = CSFX.Object;
}

void ALStepC2S002::Stop_Implementation() {
	Super::Stop_Implementation();
	UWorld* const W = GetWorld();
	if (W && IsValid(SFX_Analog)) {
		UGameplayStatics::PlaySound2D(W, SFX_Analog);
	}
}
