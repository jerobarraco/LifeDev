#include "LInteract.h"

#include "Components/AudioComponent.h"

#include "Diags/Diags.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "JUtils/Actors/CQuickMesh.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALInteract::ALInteract():Super() {
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAtt(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/SA_GenericAttenuation.SA_GenericAttenuation"));
	SFX->AttenuationSettings = CAtt.Object;

	AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	AnimFade->Meshes.Add(Mesh);
}

void ALInteract::Fade(bool FadeIn) {
	AnimFade->IsReversed = FadeIn;
	AnimFade->Play();
}

void ALInteract::BeginPlay() {
	Super::BeginPlay();
	// TODO try using PostInitProperties or PostInitializeComponents. PostLoad happens before the construction and PostActorCreated is before bps and properties maybe.
	if (!ULockItem.IsNone() || !ULockItemReq.IsNone() || !ULockFlagReq.IsNone()) {
		// note only setting it if the ulock is set.
		Locked = true;
	}

	// don't set the fade material if not NECESSARY.
	// atm i have 2 materials and this will break new objects with the new material until
	// the transition is finished. But also is unnecessary code, with unnecessary memory.
	// and could potentially slow rendering.
	// do not deactivate nor clear the meshes since that could break objects that reuse the AnimFade
	if (WillRewardFade()) {
		AnimFade->CreateMaterial();
		// avoid getting the sound killed on self-destroy
		// set here on purpose to allow the user to override it and self-hurt.
		UseAttachedSFX = false;
	}

	if (IsValid(RewardActor)) {
		RewardActor->SetActorHiddenInGame(true);
		AInteract* const Reward = Cast<AInteract>(RewardActor);
		if (IsValid(Reward)) {
			Reward->SetEnabled(false);
		}
	}

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	Inventory = World->GetSubsystem<UInventory>();
	Dialogs = World->GetSubsystem<UDiags>();
	Flags = World->GetSubsystem<UFlags>();
	Flashback = World->GetSubsystem<UFlashback>();
}

void ALInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	AnimFade->OnEnd.RemoveAll(this);

	Inventory = nullptr;
	Dialogs = nullptr;
	Flags = nullptr;
	Flashback = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ALInteract::DoRewards() {
	UWorld* const World = GetWorld();
	if (!World) return;

	/// Rewards
	const bool Rewardless = IsRewardless();
	if (Rewardless) {
		return; // just return to avoid self destroying
	}

	if (Flashback) {
		Flashback->ModVal(RewardFlash);
	}

	// reward an item if possible ( the check for IsNone is to avoid return when none)
	if (!RewardItem.IsNone() && IsValid(Inventory)) {
		// return if we fail to reward
		// for example on maxed-out (e.g. picked up consumables)
		if (!Inventory->Mod(RewardItem, 1)) return;
		// given this return will cancel the effect, do before the rest.
	}
	
	// do the flags which are more flexible.
	if (IsValid(Flags)) {
		Flags->Mod(RewardFlag, 1.0);
	}

	// do the actor
	if (IsValid(RewardActor)) {
		RewardActor->SetActorHiddenInGame(false);
		ALInteract* const RewardInter = Cast<ALInteract>(RewardActor);
		if (IsValid(RewardInter)) {
			RewardInter->SetEnabled(true);
			RewardInter->Fade(true);
		}
	}

	// not necessary to call "disable while anim = false" here.
	// since it's up to the client to allow re-triggerables.
	// and we're only concerned with SetEnabled(false) here. 
	// and it's easier and clearer this way than messing with DisableWhileAnim which
	// would step on the client's intention.
	// avoid re-rewarding due to multi clicks
	SetEnabled(false);

	// trigger separately since sometimes the item could not be rewarded on trigger.
	Rewarded();

	// Process auto destroy. do at the end.
	if (!UseRewardFade) return;
	
	// only bind here as we only want to destroy on reward
	AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteract::RewardFaded);
	Fade(false);
}

void ALInteract::Trigger_Implementation() {
	Super::Trigger_Implementation();

	// trigger the dialog anyway.
	if (IsValid(Dialogs)) {
		Dialogs->AddId(TriggerDlg);
	}
	
	DoRewards();
}

void ALInteract::RewardFaded() {
	AnimFade->OnEnd.RemoveDynamic(this, &ALInteract::RewardFaded);
	Destroy();
}

void ALInteract::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	
	if (!Inventory || !Dialogs) return;

	const bool Has = Inventory->Has(ULockItem);
	const FName& Dlg = Has && (!LockedItemDlg.IsNone())? LockedItemDlg : LockedDlg;
	FDialog D; FDialogChar C;
	Dialogs->AddId(Dlg);
}

bool ALInteract::TryTrigger_Implementation() {
	// handle item req
	if (!ULockItemReq.IsNone()) {
		const bool Ok = IsValid(Inventory) && Inventory->Has(ULockItemReq);
		if (Ok) {
			Locked = false;
		}
	}

	// handle flag req
	if (!ULockFlagReq.IsNone()) {
		const bool Ok = IsValid(Flags) && Flags->Has(ULockFlagReq);
		if (Ok) {
			Locked = false;
		}
	}
	
	return Super::TryTrigger_Implementation();
}

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Item) {
	if (Item == NAME_None) {
		UE_LOG(LogTemp, Warning, TEXT("TryUseItem with item none. cancelled."));
		return EItemUseResult::BAD_TARGET;
	}

	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool ValidDiags = IsValid(Dialogs);

	/// Say something about it
	
	// 1st check if it's a regular item. since we don't care about lock at that stage
	// and only happens if it's specified on UseItemDlgs
	// im pretty sure this will break something else. but this needs improving later
	{
    	// check if we can say something about this
    	const FName* const pDlg = UseItemDlgs.Find(Item);
		if (pDlg) {
    		const bool Added = pDlg && ValidDiags && Dialogs->AddId(*pDlg);
			// assume this is not ULockItem. if you added the same item to both places then that's wrong.
			// Using bad_handled since we don't want to consume an item.
			// this is only to say something about the item.
			// if this item needs to be consumed, use ulockItem 
    		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
		}
    }

	// if it's not locked, we need not do anything with it. don't consume it.
	// there's no other functionality to TryUseItem than saying something or unlocking (implies consuming)
	if(!Locked) {
		return EItemUseResult::BAD_TARGET;
	}

	/// Unlock with item - at this point is locked
	
	// Checks if it needs an item to unlock it. and unlock if needed.
	const bool LockBad = Item != ULockItem;
	if (LockBad) {
		const bool Added = ValidDiags && Dialogs->AddId(ULockBadDlg);
        return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	// now unlocked
	if (ValidDiags) {
		Dialogs->AddId(ULockDlg);
	}

	Locked = false; // force unlock or trigger won't work
	Trigger(); // force trigger
	return EItemUseResult::SUCCESS;
}
