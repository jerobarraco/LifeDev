// Copyright Jerónimo Barraco-Mármol

#include "BooksI02.h"

ABooksI02::ABooksI02():Super(1, 0) {
	Texts = { NSLOCTEXT("BooksI02", "State", "A book") };
	
	UseRewardDestroy = false;
	UseAnim = false;
	IsLocked = false;
	IsOneShot = true;
	UseAutoActivate = true;
	HintCondition = "{Inter.Locked.TapeI02}";
	UseHint = true;
	
	RewardActor = TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.BattsI00_UAID_D8BBC116E5012EC501_1469610310"));

	// hint the batteries in case teh user don't see it. can happen due to fb
	RewardIntersHint = {TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.BattsI00_UAID_D8BBC116E5012EC501_1469610310"))};
}
