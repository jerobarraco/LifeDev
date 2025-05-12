// Copyright Jerónimo Barraco-Mármol

#include "TapeI01.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ATapeI01::ATapeI01():Super() {
	UseAutoActivate = true;
	Texts = { NSLOCTEXT("TapeI01", "State0", "Pick up tape") };
	// TriggerDlg = FName("T1_T"); // will say what it is. triggered on pick up
	StateNum = 1;
	UseAnim = false;
	UseFade = true;
	UseRewardDestroy = true;
	RewardFlash = .1;
	RewardItem = LDConsts::Items::Tape1;
	RewardIntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI01_UAID_D8BBC116E501FCCD01_1261387011"))
	};

	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
