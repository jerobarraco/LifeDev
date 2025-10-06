// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S008.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S008::ALStepC4S008():Super() {
	Label = FName("C4S8");
	UseRain = true;
	// DlgId = FName("C4S1");
	InputEnabled = true;
	UsePawnCam = true;
	TeleportChar = false;
	FinishFlags = {"Inter.Trigger.SpotI03"};
	UseFBRand = true; // min is limited in c4s0
	RandFB->DelayMin = .5;
	RandFB->DelayMax = .85;
	RandFB->ValueMin = -.010;
	RandFB->ValueMax = .005;
	// spot
	IntersActivate = {TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E501FFE301_1677671777"))};
	ActorsShow = {TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.CharNRG_UAID_D8BBC116E50193F001_1481028493"))};
	ActorsHide = {
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LWorldText_UAID_D8BBC116E501E59502_1910779143")),
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LWorldText_UAID_D8BBC116E501E69502_1636614320")),
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LWorldText_UAID_D8BBC116E501E69502_1749170321")),
	};
}

// allows the player to sit on the table. enables a spot (set on editor)
