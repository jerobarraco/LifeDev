// Copyright Jerónimo Barraco-Mármol

#include "TapeI01.h"

ATapeI01::ATapeI01():Super() {
	Texts = { FText::FromString(TEXT("Pick up tape")) };
	TriggerDlg = FName("T0_T"); // will say what it is. triggered on pick up
	StateNum = 1;
	UseAnim = false;
	UseFade = true;
	RewardFlash = .1;
	RewardItem = "T00";
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
