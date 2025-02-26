// Copyright Jerónimo Barraco-Mármol

#include "TapeI00.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ATapeI00::ATapeI00():Super() {
	Texts = { FText::FromString(TEXT("Pick up tape")) };
	TriggerDlg = "T01_T"; // will say what it is. triggered on pick up
	RewardItem = LDConsts::Items::Tape1; // "T01";
	UseFade = true;
	UseRewardDestroy = true;
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
