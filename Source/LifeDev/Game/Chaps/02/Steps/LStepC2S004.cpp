// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S004.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC2S004::ALStepC2S004():Super() {
	static const FName N("C2S4");
	Name = N;
	// DlgId = N; // this chapter finishes after the dialog
	// no waits
	UseFade = false;
	FinishPostWait = false;
	// returns to the player camera and control
	InputEnabled = true;
	UsePawnCam = true;
	// teleport after the boss interaction happens
	TeleportChar = false;
	UseRain = true;
	// finish when found.
	// the tape is not necessary since that's checked on a previous step and also consumed.
	FinishItems = {
		LDConsts::Items::Card2,
		LDConsts::Items::Poem2,
	};
	IntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath(
			"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card02_UAID_D8BBC116E501DFC401_1977970429")),
		TSoftObjectPtr<AInteract>(FSoftObjectPath(
			"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PoemI02_UAID_D8BBC116E5012CC501_1287868959"))
	};

	// will decrease the fb. done in this step since this step could potentially last a while until they find the
	// required items, and it's hard to find them with a fb
	RandFB->DelayMin = .5;
	RandFB->DelayMax = 2;
	RandFB->ValueMin = -.07;
	RandFB->ValueMax = .02; // you wouldn't think is so easy to get out of a flashback, do you?
	// .02 is actually very generous
	UseFBRand = true;
}

void ALStepC2S004::TryStart_Implementation() {
	Super::TryStart_Implementation();
	FB->SetMin(0);
}
