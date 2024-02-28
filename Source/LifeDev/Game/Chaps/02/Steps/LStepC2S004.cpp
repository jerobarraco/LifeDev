// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S004.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC2S004::ALStepC2S004():Super() {
	static const FName N("C2S4");
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
	// finish when found.
	// the tape is not necessary since that's checked on a previous step and also consumed.
	ItemsFinish = {
		LDConsts::Items::Card2,
		LDConsts::Items::Poem2,
	};

	// will decrease the fb. done in this step since this step could potentially last a while until they find the
	// required items, and it's hard to find them with a fb
	RndFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RndFB"));
	RndFB->SetAutoActivate(false);
	RndFB->DelayMin = .5;
	RndFB->DelayMax = 2;
	RndFB->ValueMin = -.07;
	RndFB->ValueMax = .02; // you wouldn't think is so easy to get out of a flashback, do you?
	// .02 is actually very generous 
}

void ALStepC2S004::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetMin(0);
	RndFB->Activate(true);
	ALMusicMan::SetRainS(GetWorld(), true);
}

void ALStepC2S004::Stop_Implementation() {
	RndFB->Deactivate();
	FB->SetMin(0);
	FB->SetMax(1);
	FB->SetVal(0);
	ALMusicMan::SetRainS(GetWorld(), false);
	Super::Stop_Implementation();
}
