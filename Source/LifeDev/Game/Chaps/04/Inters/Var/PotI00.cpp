// Copyright Jerónimo Barraco-Mármol

#include "PotI00.h"

#include "Inventory/Inventory.h"
#include "Story/Story.h"

APotI00::APotI00():Super() {
	// RewardFlash = 0.1;
	Texts = {FText::FromString(TEXT("Hot pot")), FText::FromString(TEXT("Empty pot")) };
	UseRewardFade = false;
	Locked = false;
	LockedDlg = "Pot00_L";
	TriggerDlg = "Pot00_T";
	// IsOneShot = true; // not one shot since we need to use the items on it
	SetEnabled(false);
	// I'm using SetEnabled instead of Lock because these things will be changing during the chapter
	// and i think that the player will find easier to tell when something became enabled. 
	// than realizing something became unlocked.
	// (without any extra cue/feedback which i'm not going to add now)
	Step = 0;
}

void APotI00::BeginPlay() {
	Super::BeginPlay();
	// setState below would re-enable the object and we don't want that
	DisableWhileAnim = false;
	SetState(1); // start open
}

void APotI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	if (Step==0) {
		Locked = true;
		// locked so that we can still use the items on it.
		RewardInterEnable.Empty(); // forget about the stove. for the next interaction.
		Step = 1;
	} // TODO step 01??
}

EItemUseResult APotI00::TryUseItem_Implementation(const FName& Name) {
	// TODO check that this actually works.
	if (Name == "Food00" || Name == "Food01") {
		const bool Ok = Inventory->Use(Name);
		if (Ok) ++Foods;
		if (Foods == 2) {
			++Step; // TODO do i even need this?
			Story->StartNext();// TODO test
		}
		return Ok ? EItemUseResult::SUCCESS : EItemUseResult::BAD_HANDLED;
	}

	// TODO the plates
	return Super::TryUseItem_Implementation(Name);
}

// on editor. rewardinterenabled. enables the stove.

// 2 interactions
// 1st enable the stove and lock itself
// 2nd wait for items to be used and trigger dialog for plates?
