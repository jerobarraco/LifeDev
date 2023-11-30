// Copyright Jerónimo Barraco-Mármol

#include "BattsI00.h"

ABattsI00::ABattsI00():Super() {
	Texts = { FText::FromString(TEXT("Pick up batteries")) } ;
	RewardFlash = .1;
	RewardItem = "BattsI00";
	TriggerDlg = "BattsI00Pick";
	// this is broken somehow
	SetEnabled(true);
	// needs to move to be able to be attached to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
