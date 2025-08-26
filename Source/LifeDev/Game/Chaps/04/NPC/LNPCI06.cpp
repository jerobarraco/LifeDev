// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI06.h"

ALNPCI06::ALNPCI06():Super() {
	Texts = { NSLOCTEXT("LNPCI06", "State0", "Talk")};

	// TriggerDlg = "NPCI06_T";
	UseRewardDestroy = false;
	IsLocked = false;
	IsOneShot = true;
	UseFade = true;
	UseAnim = false;
	SetActorHiddenInGame(true);
	
	// RewardIntersActiveClass = { AStoveI00::StaticClass(), AMatchBoxI00::StaticClass() };
	// UseAutoActivate = false; // disabled by default. does not matter. get enabled by c4s2. // redundant
	RewardIntersActive = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E50190DA01_1820937732")),
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.MatchBox_UAID_D8BBC116E501173502_2139923511"))
	};
}
