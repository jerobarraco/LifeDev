// Copyright Jerónimo Barraco-Mármol

#include "BattsI00.h"

ABattsI00::ABattsI00():Super() {
	Texts = { NSLOCTEXT("BattsI00", "State0", "Pick up batteries") };
	UseHint = true;
	HintCondition = "{Inter.Locked.WalkmanI00}";

	// needs to move to be able to be attached to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
