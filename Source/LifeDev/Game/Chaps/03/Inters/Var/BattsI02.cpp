// Copyright Jerónimo Barraco-Mármol

#include "BattsI02.h"

ABattsI02::ABattsI02():Super() {
	Texts = { NSLOCTEXT("BattsI02", "State0", "Pick up batteries") };
	UseHint = true;
	SetActorHiddenInGame(true);
	RewardIntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI06_UAID_D8BBC116E5014A2C02_1407893984"))};
}
