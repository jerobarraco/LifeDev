// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S003.h"

#include "Kismet/GameplayStatics.h"

#include "Inventory/Inventory.h"

#include "LifeDev/Core/Consts/ConstItems.h"

// this step is only to show a dialog on picking up the tape and starting the rain + playing a sound
// it could be inside the tape ... but this is ok also.

ALStepC2S003::ALStepC2S003():Super() {
	static const FName N("C2S3");
	Name = N;
	DlgId = N; // this chapter finishes after the dialog
	UseFade = false;
	// returns to the player camera and control
	InputEnabled = true;
	UsePawnCam = true;
	TeleportChar = false;
	// no waits
	FinishPostWait = false;
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Generic/Analog/Analog_C.Analog_C"));
	SFX_Analog = CSFX.Object;
}

void ALStepC2S003::TryStart_Implementation() {
	// just remove the tape, since implementing the using of it is rather complex.
	// the user interaction is not in place yet, so it won't be a good exp.
	// we can't pick it up without the batteries, so story wise is safe.
	// note apparently modding inventory while a dialog is shown could show the inventory below the dialog
	Inventory->Mod(LDConsts::Items::Tape2, -1);

	// super will start the dialog and finish when done
	Super::TryStart_Implementation();

	const UWorld* const W = GetWorld();
	if (LIKELY(W) && IsValid(SFX_Analog)) UGameplayStatics::PlaySound2D(W, SFX_Analog);
}
