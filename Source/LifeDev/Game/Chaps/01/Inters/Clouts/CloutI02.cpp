// Copyright Jerónimo Barraco-Mármol

#include "CloutI02.h"

ACloutI02::ACloutI02():Super() {
	RewardItem = TEXT("C1C02"); // chapter 1 clout 3
	RewardIntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Spot00_UAID_D8BBC116E5019D8C01_1333541631"))
	};
}
