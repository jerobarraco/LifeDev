// Copyright Jerónimo Barraco-Mármol

#include "BooksI01.h"

ABooksI01::ABooksI01() {
	Texts = { NSLOCTEXT("BooksI01", "State", "A book") };
	// TriggerDlg = "BK01_T";
	BookCount = 1;
	UseRewardDestroy = false;
	IsOneShot = true;
	IsLocked = false;
	UseAutoActivate = true;
	RewardActor = TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card04_UAID_D8BBC116E501FB3602_1428206695"));
}
