// Copyright Jerónimo Barraco-Mármol

#include "TapeI02.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ATapeI02::ATapeI02():Super() {
	Texts = { FText::FromString(TEXT("Another tape of mine")) };
	StateNum = 1;
	UseAnim = false;
	RewardFlash = .1;
	RewardItem = "T02";
	UseRewardFade = true;
	Locked = true;
	ULockItem = LDConsts::Items::Batts;
	LockedDlg = FName("T02_L");
	TriggerDlg = FName("T02_Look*"); // will say what it is. triggered on pick up
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
	// At the moment it says the whole song on look or pick.
	// TODO leave only the 1st dialog as look or pick. then show the rest on item usage.
}
