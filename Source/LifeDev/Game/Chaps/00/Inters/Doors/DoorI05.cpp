// Copyright Jerónimo Barraco-Mármol

#include "DoorI05.h"

#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "Story/Story.h"

ADoorI05::ADoorI05():Super() {
	Locked = true;
	UseAnim = false; // will take us directly to the next story step
	UseAutoActivate = true;
	IsOneShot = true;
	RewardStep = true;
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

void ADoorI05::DoTrigger_Implementation() {
	static FName Step("C0S0");
	Super::DoTrigger_Implementation();

	if (!IsValid(Story)) return;

	// SetActive(false); // avoid double triggering. done only if story is valid.
	// Story->StartNext(Step);
}
