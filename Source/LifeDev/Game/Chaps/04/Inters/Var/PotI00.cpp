// Copyright Jerónimo Barraco-Mármol

#include "PotI00.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "Story/Story.h"

// deactivated by default
// stove activates it.
// two foods unlock and triggers.
// then goes to next step.
// the step enables the sponge.
// the sponge goes to next step and gives plate.
// the plate can be used again which rewards another plate.
// the other plate triggers the spot.

// todo i got tired of this class bringing problems for being such a snowflake,
//	try to find a different way to implement it.

APotI00::APotI00():Super() {
	RewardItem = NAME_None;
	UseRewardDestroy = false;
	UseFade = false;
	UseAutoActivate = false; // activated by the step

	// IsOneShot = true; // not one shot since we need to use the items on it
	// UseAutoActivate = false; // enabled by the stove // redundant
	// RewardFlash = 0.1;

	// Override the states and transforms
	// intentionally letting it loop to empty after done.
	// so that, using the plates, open the pot
	// 0: boiling
	// 1: rice and egg added
	// 0: empty
	StateNum = 2;
	Texts = {
		NSLOCTEXT("PotI00", "State0", "Boiling ..."),
		NSLOCTEXT("PotI00", "State1", "Cooking ..."),
	};
	const FRotator State0Rot(0, -10, 0);
	Trans = {
		FTransform(State0Rot),
		FTransform(),
	};
	Lid->SetRelativeRotation(State0Rot); // init the correct transform
	Anim->IsAdditive = false;
	// preload
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd2(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/water_dropped_on_electric_stove_02_edit"));
	SNDDrops = CSnd2.Object;

	// first interaction is unlocking food
	// locked so that player can't trigger manually,
	// but they can still use the items on it.
	IsLocked = true;
	IsOneShot = false;
	// TODO this will be a bit more complicated
	TriggerDlg = "Pot00.0_T"; // todo should be Pot00_T.0 instead
	// LockedDlg = "Pot00_L.0"; // handled by data table
	UnlockItems = { "Food00", "Food01" };
	SFXs = {SNDDrops, nullptr};
}

void APotI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	UE_LOG(LogTemp, Log, TEXT("%hs state=%i"), __func__, State);

	// this could potentially be SetState instead of DoTrigger, but it's possible that SetState(0) is called on beginplay

	// state ought to be the new one after super::doTrigger (that means that the first time it's going to be 1)
	if (State == 1) {
		// triggered after adding food
		// all this only affects the next trigger (using the plate) for next trigger (plates)
		TriggerDlg = "Pot00.1_T"; // clear the trigger dialog for next step
		// LockedDlg = "Pot00.1_L";
		UnlockItems = {LDConsts::Items::Plate01};
		IsLocked = true;
		// story is advanced by the step
		// Story->StartNext(); // manually advance.
	} else if (State == 0) { // has looped over (notice the check is last)
		RewardItem = LDConsts::Items::Plate02;
		IsOneShot = true; // no more triggers after this
		// not advancing the story here. it will advance when the player uses the plate on the chair (spot)
	}
}
