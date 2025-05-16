// Copyright Jerónimo Barraco-Mármol

#include "PotI00.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Inventory/Inventory.h"
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

APotI00::APotI00():Super() {
	RewardItem = NAME_None;
	UseRewardDestroy = false;
	UseFade = false;
	UseAutoActivate = false; // activated by the step

	// locked so that player can't trigger manually,
	// but they can still use the items on it.
	IsLocked = true;
	TriggerDlg = "Pot00.0_T";
	LockedDlg = "Pot00.0_L";
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

	SFXTrigger = nullptr;
	// preload
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd2(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/water_dropped_on_electric_stove_02_edit"));
	SND_Drops = CSnd2.Object;
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
		LockedDlg = "Pot00.1_L";
		SFXTrigger = SND_Drops;
		Story->StartNext(); // manually advance.
	} else if (State == 0) { // has looped over (notice the check is last)
		// reward a plate. not using rewarditem or the parent's functionality since it's too cumbersome in this case.
		Inventory->Mod(LDConsts::Items::Plate02, 1);
		// Could set the text here. but since it's deactivated it does not matter.
		SFXTrigger = nullptr; // no sound after
		// not advancing the story here. it will advance when the player uses the plate on the chair (spot)
	}
}

EItemUseResult APotI00::TryUseItem_Implementation(const FName& Name) {
	// only observe these items
	// returning success will "consume" the items. (good)
	if (State == 0 && (Name == "Food00" || Name == "Food01")) {
		++Foods;
		// to advance the state. Trigger skips the lock check (instead of TryTrigger)
		if (UNLIKELY(Foods == 2)) Trigger();
		return EItemUseResult::SUCCESS;
	} else if (State == 1 && (Name == LDConsts::Items::Plate01)) {
		// no more interaction for you.
		// can't disable on DoTrigger since AInteractAnim will re-enable since the disablewhileanim.
		// i mean, i could move this there, but i don't want to have 2 (confusing) if statements.
		IsOneShot = true;
		Trigger();
		return EItemUseResult::SUCCESS;
	}

	// calling super to handle correctly
	return Super::TryUseItem_Implementation(Name);
}
