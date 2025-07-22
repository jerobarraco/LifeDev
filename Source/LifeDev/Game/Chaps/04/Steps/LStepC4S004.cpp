// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S004.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S004::ALStepC4S004():Super() {
	Name = FName("C4S4");
	// DlgId = FName("C4S1");
	UseRain = true;
	InputEnabled = true;
	FinishPostWait = false;
	UsePawnCam = true;
	TeleportChar = false;
	FinishFlags = { "Inter.Trigger.PotI00" }; // finish when the pot is triggered

	UseFBRand = true; // min is limited in c4s0
	RandFB->DelayMin = .5;
	RandFB->DelayMax = .85;
	RandFB->ValueMin = -.015;
	RandFB->ValueMax = .005;
	IntersActivate = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E501C1E701_2080650083"))
		// this one does not work
		// TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game /__ExternalActors__/LifeDev/Game/Sys/Game_L/9/UP/CQK85FYMK0W4PWZQYSXILC.uasset"))
	};
}

// enables the fridge and rice and pot
