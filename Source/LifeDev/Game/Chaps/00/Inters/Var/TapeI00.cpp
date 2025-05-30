// Copyright Jerónimo Barraco-Mármol

#include "TapeI00.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ATapeI00::ATapeI00():Super() {
	UseAutoActivate = true;
	Texts = { NSLOCTEXT("TapeI00", "State0", "Pick up tape") };
	// TriggerDlg = "T00_T"; // will say what it is. triggered on pick up
	RewardItem = LDConsts::Items::Tape0;
	UseFade = true;
	UseRewardDestroy = true;
	UseHint = true;
	HintCondition = "{Inter.Locked.RadioI00}"; // once it tried to use the radio
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
