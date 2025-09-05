// Copyright Jerónimo Barraco-Mármol

#include "BattsI00.h"

ABattsI00::ABattsI00():Super() {
	Texts = { NSLOCTEXT("BattsI00", "State0", "Pick up batteries") };
	UseHint = true;
	HintCondition = "{Inter.Locked.WalkmanI00}";
	RewardIntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.B_Walkman_C_UAID_D8BBC116E501248901_1198390170"))
	};
	// needs to move to be able to be attached to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
