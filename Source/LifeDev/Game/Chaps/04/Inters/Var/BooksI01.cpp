// Copyright Jerónimo Barraco-Mármol

#include "BooksI01.h"

ABooksI01::ABooksI01() {
	BookCount = 1;
	RndSeed = 6;

	Texts = { NSLOCTEXT("BooksI01", "State", "A book") };
	// TriggerDlg = "BK01_T";
	UseAutoActivate = true;
	UseRewardDestroy = false;
	UseHint = true; // hint at the start.
	IsOneShot = true;
	IsLocked = false;
	RewardActor =
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI05_UAID_D8BBC116E501A29402_1284366295"));
}
