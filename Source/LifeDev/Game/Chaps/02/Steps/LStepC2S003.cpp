// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S003.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC2S003::ALStepC2S003():Super() {
	static const FName N("C2S3");
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
	// finish when found. not many items 
	FinishItems = {
		LDConsts::Items::Poem2,
		LDConsts::Items::Card2,
	};
	
	RndFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RndFB"));
	RndFB->SetAutoActivate(false);
	RndFB->DelayMin = 1;
	RndFB->DelayMax = 4;
	RndFB->ValueMin = -.1;
	RndFB->ValueMax = .02; // you wouldn't think is so easy to get out of a flashback, do you?
}


void ALStepC2S003::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetMin(0);
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStepC2S003::AfterDlg);
	Dialogs->AddId(Name); // say something about the tape
}

void ALStepC2S003::AfterDlg() {
	Dialogs->OnDone.RemoveAll(this);
	// just remove since implementing the using of it is rather complex.
	// the user interaction is not in place yet so it wont be a good exp
	// we can't pick it up without the batteries, so story wise is safe.
	Inventory->Mod("T02", -1);
	ALMusicMan::SetRainS(GetWorld(), true);
	RndFB->Activate(true);
}

void ALStepC2S003::Stop_Implementation() {
	ALMusicMan::SetRainS(GetWorld(), false);
	RndFB->Deactivate();
	FB->SetVal(.1);
	Super::Stop_Implementation();
}
