// Copyright Jerónimo Barraco-Mármol

#include "PoemI02.h"

APoemI02::APoemI02():Super() {
	RewardFlash = -.2f;
	RewardItem = "P02";
	TriggerDlg = "P02_Look";

	
	// Can't be static since it will be inside of a drawer for example
	Super::SetMobility(EComponentMobility::Movable);
}
