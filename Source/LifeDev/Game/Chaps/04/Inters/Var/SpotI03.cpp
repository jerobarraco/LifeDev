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
	RewardStep = true; // moves to the next

	StateNum = 1;
	Items = { LDConsts::Items::Plate02 }; // empty plate
	Texts = { FText(NSLOCTEXT("Chap02", "Spot02.Sit", "Sit here")) };

	Super::SetAutoActivate(false); // the steps will enable it
	// RewardActor set in editor
}
