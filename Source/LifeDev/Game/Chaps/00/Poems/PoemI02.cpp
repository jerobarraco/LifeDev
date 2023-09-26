// Copyright Jerónimo Barraco-Mármol

#include "PoemI02.h"

APoemI02::APoemI02():Super() {
	ItemReward = "P02";
	TriggerDlg = "P02_T";
	TriggerFlashInc = -.2f;

	
	// Can't be static since it will be inside of a drawer for example
	Super::SetMobility(EComponentMobility::Movable);
}
