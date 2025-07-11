// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC2S002.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC2S002::ALStepC2S002():Super() {
	static const FName N("C2S2");
	Name = N;
	UseFade = false;
	// returns to the player camera and control
	InputEnabled = true;
	UsePawnCam = true;
	TeleportChar = false;

	// no waits
	FinishPostWait = false;
	// wait for the tape (important the next step lowers the fb to allow to find other items)
	FinishItems = { LDConsts::Items::Tape2 };
	RemoveItems = { LDConsts::Items::Batts };
	IntersFadeIn = { // fade in so that increasing the step does not hide them
		TSoftObjectPtr<ALInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI00_UAID_D8BBC116E501E2CD01_1084873435"))};
	IntersHint = { // tape
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.TapeI02_UAID_D8BBC116E5012DC501_2098743131"))};
}

void ALStepC2S002::BeginPlay() {
	Super::BeginPlay();

	// force fade the card
	DoIntersFade(IntersFadeIn, false);
}
