// Copyright Jerónimo Barraco-Mármol

#include "SpotI03.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ASpotI03::ASpotI03():Super() {
	// DropDlg = TEXT("IS0_D*");
	TriggerDlg = "IS3_T"; // when completed
	LockedDlg = "IS3_L"; // when not full, try trigger
	LockedFullDlg = ""; // when full, try trigger. after 1st trigger.
	DropDlg = "";
	DropFullDlg = ""; // when full
	RewardStep = true; // moves to the next
	Items = { LDConsts::Items::Plate02 }; // empty plate

	SetEnabled(false); // the steps will enable it
	// RewardActor set in editor
}
