// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S002.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC2S002::ALStepC2S002():Super() {
	static const FName N("C2S2");
	Name = N;
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
	ItemsFinish = { "T02", LDConsts::Items::Card2 };
}
