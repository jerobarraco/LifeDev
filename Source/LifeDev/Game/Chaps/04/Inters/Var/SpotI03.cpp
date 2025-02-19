// Copyright Jerónimo Barraco-Mármol

#include "SpotI03.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI03::ASpotI03():Super() {
	// DropDlg = TEXT("IS0_D*");
	TriggerDlg = "IS3_T"; // when completed
	LockedDlg = "IS3_L"; // when not full, try trigger
	LockedFullDlg = ""; // when full, try trigger. after 1st trigger.
	DropDlg = "";
	DropFullDlg = ""; // when full
	RewardStep = true; // moves to the next step on done.

	Items = { LDConsts::Items::Plate02 }; // empty plate

	Super::SetAutoActivate(false); // the steps will enable it
	// RewardActor set in editor
}
