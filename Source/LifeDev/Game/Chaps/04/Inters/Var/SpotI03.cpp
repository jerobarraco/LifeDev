// Copyright Jerónimo Barraco-Mármol

#include "SpotI03.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI03::ASpotI03():Super() {
	// TriggerDlg = "IS3_T"; // when completed
	UnlockItems = { LDConsts::Items::Plate02 }; // empty plate
	IsOneShot = true;
	UseHint = true;
	HintCondition = "{V.Item.Count.Plate02}"; // only after getting the plate with food
	// LockedDlg = "IS3_L"; // when not full, try trigger
	// UseAutoActivate = false; // the steps will enable it // redundant
	// RewardActor plate
	RewardActor = TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E5010EE301_1350177374"));
}
