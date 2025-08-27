// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S002.h"

#include "LifeDev/Game/Flashback/CRandomizerFB.h"

ALStepC4S002::ALStepC4S002():Super() {
	Label = FName("C4S2");
	// DlgId = FName("C4S1");
	UseRain = true;
	InputEnabled = true;
	UsePawnCam = true;
	TeleportChar = false;
	FinishFlags = { "Inter.Trigger.StoveI00" };

	UseFBRand = true; // min is limited in c4s0
	RandFB->DelayMin = .5;
	RandFB->DelayMax = .85;
	RandFB->ValueMin = -.015;
	RandFB->ValueMax = .005;
	ActorsShow = {
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.CharNRG_UAID_D8BBC116E50193F001_1688570496"))
	};
	IntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC02_UAID_D8BBC116E501A1E401_1122317285"))
	};
	IntersActivate = {TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC02_UAID_D8BBC116E501A1E401_1122317285"))};
}

