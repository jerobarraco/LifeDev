// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S002.h"

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
	// wait for the tape (important the next step lowers the fb to allow to find other items)
	ItemsFinish = {"T02"};
}

void ALStepC2S002::Stop_Implementation() {
	// this is the card. using this so i don't have to code the fade in
	// setting to null to avoid getting it destroyed by the step
	// though technically unreal will nullify this for me,
	// and i always check for IsValid instead of null
	// once the player obtains the interact and gets destroyed;
	// there's a lot of chance of problems due to timing issues.
	// specially since this step finishes with Card2 obtained.
	Actor = nullptr;

	Super::Stop_Implementation();
}
