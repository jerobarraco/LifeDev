// Copyright Jerónimo Barraco-Mármol

#include "PlantI00.h"

// #include "Interact/CInteract.h"

APlantI00::APlantI00():Super() {
	// TriggerDlg = "PL00_T";
	// the plant on c03 has some batteries. the box collides with the batts.
	// but i've fixed that by setting the plant to oneShot.
	IsOneShot = true;
	UseAnim = false;
	DisableWhileAnim = false;
	UseAutoActivate = true;
	// Interact->SetBoxExtent(FVector(37.500000,37.500000,27.500000));
	// Interact->SetRelativeLocation(FVector(32.500000,-32.500000,30));

	RewardIntersHint = { // battsc3
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.BattsI00_UAID_D8BBC116E501B60902_2059788063"))
	};
}
