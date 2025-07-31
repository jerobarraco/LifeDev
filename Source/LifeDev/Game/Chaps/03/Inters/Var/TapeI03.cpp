// Copyright Jerónimo Barraco-Mármol

#include "TapeI03.h"

#include "LifeDev/Core/Consts/ConstItems.h"

// This is a requirement for the last step
// rewarded by puzzlei06
ATapeI03::ATapeI03():Super() {
	Texts = { NSLOCTEXT("TapeI03", "State0", "One more tape") };
	StateNum = 1;
	UseAnim = false;
	RewardFlash = -.1;
	RewardItem = LDConsts::Items::Tape3;
	UseRewardDestroy = true;
	UseFade = true;
	IsLocked = true;
	UseAutoActivate = true; // rewarded by the puzzle.
	// autoActivate works well, and it's needed so it gets activated on reward.

	UnlockItems = { LDConsts::Items::Batts };
	// LockedDlg = FName("T03_L");
	// LockedItemDlg = FName("T03_LI"); // will hint to use the batts
	// TriggerDlg = FName("T03_Look"); // will say what it is. triggered on pick up
}
