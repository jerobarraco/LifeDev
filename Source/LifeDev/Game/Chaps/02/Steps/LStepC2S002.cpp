// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S002.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

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
	// finish when found. not many items 
	FinishItems = {
		"T02",
		LDConsts::Items::Poem2,
		LDConsts::Items::Card2,
	};
}
