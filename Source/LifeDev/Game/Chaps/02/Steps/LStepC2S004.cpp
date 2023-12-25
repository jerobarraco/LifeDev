// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S004.h"

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
	// finish when found. not many items 
	FinishItems = {
		LDConsts::Items::Poem2,
		LDConsts::Items::Card2,
	};
}

void ALStepC2S004::Stop_Implementation() {
	FB->SetVal(0);
	Super::Stop_Implementation();
}
