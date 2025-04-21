// Copyright Jerónimo Barraco-Mármol

#include "SpotI03.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI03::ASpotI03():Super() {
	TriggerDlg = "IS3_T"; // when completed
	LockedDlg = "IS3_L"; // when not full, try trigger
	RewardStep = true; // moves to the next step on done. TODO deprecated

	Items = { LDConsts::Items::Plate02 }; // empty plate

	// UseAutoActivate = false; // the steps will enable it // redundant
	// RewardActor set in editor
}
