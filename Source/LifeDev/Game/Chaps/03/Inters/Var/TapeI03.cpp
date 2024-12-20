// Copyright Jerónimo Barraco-Mármol

#include "TapeI03.h"

#include "LifeDev/Core/Consts/ConstItems.h"

// This is a requirement for the last step
ATapeI03::ATapeI03():Super() {
	Texts = { FText::FromString(TEXT("One more tape")) };
	StateNum = 1;
	UseAnim = false;
	RewardFlash = -.1;
	RewardItem = LDConsts::Items::Tape3;
	UseRewardDestroy = true;
	UseFade = true;
	Locked = true;

	ULockItem = LDConsts::Items::Batts;
	LockedDlg = FName("T03_L");
	LockedItemDlg = FName("T03_LI"); // will hint to use the batts
	TriggerDlg = FName("T03_Look*"); // will say what it is. triggered on pick up
}
