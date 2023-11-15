// Copyright Jerónimo Barraco-Mármol

#include "TapeI00.h"

ATapeI00::ATapeI00():Super() {
	Texts = {
		FText::FromString(TEXT("Pick up tape")),
	};
	TriggerDlg = "T01_T"; // will say what it is. triggered on pick up
	RewardItem = "T01";
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
