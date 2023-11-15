// Copyright Jerónimo Barraco-Mármol

#include "TapeI00.h"

ATapeI00::ATapeI00():Super() {
	Texts = {
		FText::FromString(TEXT("Pick up tape")),
	};
	RewardItem = "T01";
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
