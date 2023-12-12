// Copyright Jerónimo Barraco-Mármol

#include "TapeI02.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ATapeI02::ATapeI02():Super() {
	Texts = { FText::FromString(TEXT("Pick up tape")) };
	TriggerDlg = FName("T02_Look"); // will say what it is. triggered on pick up
	StateNum = 1;
	UseAnim = false;
	RewardFlash = .1;
	RewardItem = "T02";
	Locked = true;
	ULockItem = LDConsts::Items::Batts;
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
