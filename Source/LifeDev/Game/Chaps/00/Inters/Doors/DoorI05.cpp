// Copyright Jerónimo Barraco-Mármol

#include "DoorI05.h"

#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ADoorI05::ADoorI05():Super() {
	IsLocked = true;
	UseAnim = false; // will take us directly to the next story step
	UseAutoActivate = true;
	IsOneShot = true;
	RewardStep = true;
	// i'm _assuming_ printf is more performant when building strings. because i'd make it like that.
	ULockCondition = FString::Printf(TEXT("{%ls}&{%ls}&{V.Item.Count.%ls}"),
		*LDConsts::Flags::Game::CH0::Basin.ToString(),
		*LDConsts::Flags::Game::CH0::Spot.ToString(),
		*LDConsts::Items::Card0.ToString());
}

bool ADoorI05::TryTrigger_Implementation() {
	// TODO i can use autodialogs for this one (use a pick with a condition).
	// verify the state. (pull not push, just like agile).
	if (!Flags->Has(LDConsts::Flags::Game::CH0::Basin)) {
		LockedDlg = "D05_L.Basin";
	} else if (!Flags->Has(LDConsts::Flags::Game::CH0::Spot)) {
		LockedDlg = "D05_L.Spot";
	} else if (!Inventory->Has(LDConsts::Items::Card0)) {
		LockedDlg = "D05_L.C0";
	} else {
		Unlock();
	}

	return Super::TryTrigger_Implementation();
}
