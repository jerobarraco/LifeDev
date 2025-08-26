// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S006.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S006::ALStepC4S006():Super() {
	Label = FName("C4S6");
	UseRain = true;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	UsePawnCam = true;
	TeleportChar = false;
	FinishFlags = {"Inter.Trigger.SpongeI00"};
	UseFBRand = true; // min is limited in c4s0
	RandFB->DelayMin = .5;
	RandFB->DelayMax = .85;
	RandFB->ValueMin = -.015;
	RandFB->ValueMax = .005;
	ActorsShow = {TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.CharNRG_UAID_D8BBC116E50193F001_1688173495"))};
	// enables the sponge
	IntersActivate = { TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E5010BE301_2021529842"))};
	IntersHint = {TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E5010BE301_2021529842"))};
}

