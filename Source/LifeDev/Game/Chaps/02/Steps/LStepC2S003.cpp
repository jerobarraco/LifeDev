// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S003.h"

#include "Kismet/GameplayStatics.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC2S003::ALStepC2S003():Super() {
	static const FName N("C2S3");
	Name = N;
	DlgId = N;
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
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Generic/Analog/Analog_C.Analog_C"));
	SFX_Analog = CSFX.Object;
}

void ALStepC2S003::Start_Implementation() {
	// just remove the card since implementing the using of it is rather complex.
    // the user interaction is not in place yet so it wont be a good exp
    // we can't pick it up without the batteries, so story wise is safe.
	// note apparently modding inventory while a dialog is shown could show the inventory below the dialog
    Inventory->Mod("T02", -1);
	// start stuff only after the 1st dialog
	Dialogs->OnShow.AddUniqueDynamic(this, &ALStepC2S003::DlgShown);

	// super will start the dialog and finish when done
	Super::Start_Implementation();

	UWorld* const W = GetWorld();
	if (W && IsValid(SFX_Analog)) {
		UGameplayStatics::PlaySound2D(W, SFX_Analog);
	}
}

void ALStepC2S003::DlgShown(const FDialog& Diag) {
	Dialogs->OnShow.RemoveAll(this);
	
	ALMusicMan::SetRainS(GetWorld(), true);
}
