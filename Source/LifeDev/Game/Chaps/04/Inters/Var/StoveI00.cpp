// Copyright Jerónimo Barraco-Mármol

#include "StoveI00.h"

#include "LifeDev/Core/Consts/ConstItems.h"

AStoveI00::AStoveI00():Super() {
	Locked = false;
	TriggerDlg = "Stove00_T";
	LockedDlg = "Stove00_L";
	LockedItemDlg = "Stove00_LI";
	ULockItem = LDConsts::Items::Matches00;
	// RewardStep = true; // last item in the interaction chain for this step.
	Super::SetAutoActivate(false);
}
