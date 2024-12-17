// Copyright Jerónimo Barraco-Mármol

#include "PotI00.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "Story/Story.h"

APotI00::APotI00():Super() {
	// RewardFlash = 0.1;
	RewardItem = NAME_None;
	UseRewardDestroy = false;
	UseFade = false;
	Locked = true;
	ULockItem = LDConsts::Items::Matches00;
	TriggerDlg = ""; // "Pot00.0_T"; makes it confusing with the story dialog
	LockedDlg = "Pot00.0_L";
	// IsOneShot = true; // not one shot since we need to use the items on it
	Super::SetAutoActivate(false); // enabled by the npc i06.

	// Override the states and transforms
	// 0: Empty pot, lid open.
	// 1: Boiling pot, lid closed.
	// 2: Rice and mayo added.
	// intentionally letting it loop to empty after done.
	// so that using the plates open the pot and reads 'empty'
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

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/Boiling_Water_in_a_Coffee_Pot"));
	SFX_Trigger = CSnd.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd2(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/water_dropped_on_electric_stove_02_edit"));
	SND_Drops = CSnd2.Object;
}

void APotI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	UE_LOG(LogTemp, Log, TEXT("%hs state=%i"), __func__, State);
	
	// state ought to be the new one after super::doTrigger
	if (State == 1) {
		// locked so that player can't trigger manually,
		// but they can still use the items on it.
		Locked = true;
		// forget about the stove. important for the next step
		RewardIntersActive.Empty();
		TriggerDlg = ""; // clear the trigger dialog for next step
		LockedDlg = "Pot00.1_L";

		Story->StartNext(); // manually advance. stove is disabled
	} else if (State == 2) {
		// triggered after adding food, by using the empty plate
		// for next step (plates)
		RewardItem = LDConsts::Items::Plate02;
		// triggers once the empty plate is used and the full rewarded
		TriggerDlg = "Pot00.2_T";
		LockedDlg = "Pot00.2_L"; // "you'll need a plate"
		SFX_Trigger = SND_Drops;

		Story->StartNext();
	} else if (State == 0) { // has looped over (notice the check is last)
		SFX_Trigger = nullptr; // no sound after
		SetActive(false); // no more interaction for you
	}
}

EItemUseResult APotI00::TryUseItem_Implementation(const FName& Name) {
	// only observe these items
	if (State == 1 && (Name == "Food00" || Name == "Food01")) {
		++Foods;
		// to advance the state. Trigger skips the lock check (instead of TryTrigger)
		if (Foods == 2) Trigger();
		return EItemUseResult::SUCCESS;
	} else if (State == 2 && (Name == LDConsts::Items::Plate01)) {
		Trigger();
		return EItemUseResult::SUCCESS;
	} 

	return Super::TryUseItem_Implementation(Name);
}

// disabled: on editor.
// enabled by a step

// 2 interactions
// 1st enable the stove and lock itself
// 2nd wait for items to be used and trigger dialog for plates?
