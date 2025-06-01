// Copyright Jerónimo Barraco-Mármol

#include "DoorI01.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ADoorI01::ADoorI01():Super() {
	IsLocked = true;
	// for the future
	UnlockItems = {LDConsts::Items::Walkman};
	// ULockItemReq = "WM"; // the wm depends on the batts so no need to go so hard on this
	// ULockCondition = "{V.Item.Count.Batts} & {V.Item.Count.WM}";
	UseHint = true;
	RootComponent->SetWorldLocation(FVector(-290,316,0));
	RootComponent->SetWorldRotation(FRotator(0,180,0));
	RewardIntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PoemI01_UAID_D8BBC116E501C49F01_1976559643"))
	};
}
