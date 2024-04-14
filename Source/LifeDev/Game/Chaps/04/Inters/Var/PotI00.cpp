// Copyright Jerónimo Barraco-Mármol

#include "PotI00.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Inventory/Inventory.h"
#include "Story/Story.h"

APotI00::APotI00():Super() {
	// RewardFlash = 0.1;
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
	// Step = 0;

	// Override the states and transforms
	// 0: Empty pot, lid open.
	// 1: Boiling pot, lid closed.
	// 2: Rice and mayo added.
	StateNum = 3;
	Texts = {
		FText::FromString(TEXT("Empty pot")),
		FText::FromString(TEXT("Hot pot")),
		FText::FromString(TEXT("Done pot")),
	};
	const FRotator State0Rot(0, -10, 0);
	Trans = {
		FTransform(State0Rot),
		FTransform(),
		FTransform(),
	};
	Lid->SetRelativeRotation(State0Rot); // init the correct transform
	Anim->IsAdditive = false;
}

void APotI00::BeginPlay() {
	Super::BeginPlay();
}

void APotI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	UE_LOG(LogTemp, Log, TEXT("%hs state=%i"), __func__, State);
	
	// state ought to be the new one after super::doTrigger
	if (State == 1) {
		// }
		// if (Step==0) {
		// locked so that player can't trigger manually but
		// they can still use the items on it.
		Locked = true;
		// forget about the stove. important for the next step
		RewardInterEnable.Empty();
		TriggerDlg = ""; // clear the trigger dialog for next step
		// Step = 1;
	} else if (State == 2) {
		Story->StartNext();// TODO test
	}
}

EItemUseResult APotI00::TryUseItem_Implementation(const FName& Name) {
	// TODO check that this actually works.
	// TODo This is consuming both items. why?
	// only observe these items
	if (Name == "Food00" || Name == "Food01") {
		++Foods;
		if (Foods == 2) DoTrigger(); // to advance the state 
		return EItemUseResult::SUCCESS;
	}

	// TODO the plates
	return Super::TryUseItem_Implementation(Name);
}

// on editor. rewardinterenabled. enables the stove.

// 2 interactions
// 1st enable the stove and lock itself
// 2nd wait for items to be used and trigger dialog for plates?
