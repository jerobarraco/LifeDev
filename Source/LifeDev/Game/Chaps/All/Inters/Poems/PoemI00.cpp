// Copyright Jerónimo Barraco-Mármol

#include "PoemI00.h"

APoemI00::APoemI00():Super() {
	RewardItem = "P00";
	RewardFlash = -.1f;
	// TriggerDlg = "Item.Look.P00"; // todo add a new group to redirect to this dialog instead of using this var
	// UseHint = true;
	// HintCondition = "{Inter.Locked.DoorI04}>2"; // unfortunately the card is obtained after the door ;_;
	// a lazy dog is a sheet of paper
	// a sheet of paper is an ink lined plane
	// an inclined plane is a slope up
	// a slow pup is a lazy dog

	// Can't be static since it will be inside a drawer, for example
	Super::SetMobility(EComponentMobility::Movable);
}
