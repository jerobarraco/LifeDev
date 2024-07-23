#include "LInteract.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundClass.h"

#include "Diags/Diags.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "JUtils/Actors/CQuickMesh.h"
#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogLInteract, Log, Log);

ALInteract::ALInteract():Super() {
	// This attenuation has the occlusion disabled to avoid self-occlusion on play detached
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAtt(LDConsts::Audio::Attns::SFX);
	SFX->AttenuationSettings = CAtt.Object;

	static ConstructorHelpers::FObjectFinder<USoundClass>
		CSClass(LDConsts::Audio::Classes::SFX);
	SFX->SoundClassOverride = CSClass.Object;

	AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	AnimFade->Meshes.Add(Mesh);
}

void ALInteract::Fade_Implementation(bool FadeIn) {
	UE_LOG(LogLInteract, Log, TEXT("%hs in=%i o=%s"),
		__func__, FadeIn, *GetNameSafe(this));
	if (UseFade) {
		AnimFade->IsReversed = FadeIn;
		AnimFade->Activate(true);
	}

	// i'm not super sure about this.
	// probably could collide with the intention of fading something in without being enabled
	// might happen on a step auto-fading something.
	// please me from the future, be careful. "ki o tsukete!"
	SetEnabled(FadeIn);
}

void ALInteract::BeginPlay() {
	Super::BeginPlay();

	// decided not to set locked here depending on the other flags, since that actually
	// can confuse me while playing and removes the explicit-ness of the code.
	// is not that it's a bad technique, just that in this case is not helpful.

	// don't set the fade material if not NECESSARY.
	// atm i have 2 materials and this will break new objects with the new material until
	// the transition is finished. But also is unnecessary code, with unnecessary memory.
	// and could potentially slow rendering.
	// do not deactivate nor clear the meshes since that could break objects that reuse the AnimFade
	if (UseFade) AnimFade->CreateMaterial();

	// avoid getting the sound killed on self-destroy
	// set here on purpose to allow the user to override it and self-hurt.
	if (WillRewardDestroy()) {
		UseAttachedSFX = false;
		if (!UseFade)
			UE_LOG(LogLInteract, Warning, TEXT("%hs Will RewardDestroy but UseFade is false."
				"This is legal but unlikely. o=%s"), __func__, *GetNameSafe(this));
	}

	if (IsValid(RewardActor)) { // hide and disable reward actor if any.
		RewardActor->SetActorHiddenInGame(true);
		AInteract* const Reward = Cast<AInteract>(RewardActor);
		if (IsValid(Reward)) Reward->SetEnabled(false);
	}

	const UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	Inventory = World->GetSubsystem<UInventory>();
	Diags = World->GetSubsystem<UDiags>();
	Flags = World->GetSubsystem<UFlags>();
	Flashback = World->GetSubsystem<UFlashback>();
	Story = World->GetSubsystem<UStory>();
}

void ALInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	AnimFade->OnEnd.RemoveAll(this);

	Inventory = nullptr;
	Diags = nullptr;
	Flags = nullptr;
	Flashback = nullptr;
	Story = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ALInteract::DoRewards() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"),
		__func__, *GetNameSafe(this));
	Diags->OnDone.RemoveDynamic(this, &ALInteract::DoRewards);

	const UWorld* const World = GetWorld();
	if (!World) return;

	/// Rewards
	// just return. nothing to do. don't self destroy or anything.
	if (IsRewardless()) {
		UE_LOG(LogLInteract, Log, TEXT("%hs Nothing to reward. Skip. o=%s"),
			__func__, *GetNameSafe(this));
		return;
	}

	// it's not necessary to call "disable while anim = false" here.
	// since it's up to the client to allow re-triggerables.
	// and we're only concerned with SetEnabled(false) here.
	// and it's easier and clearer this way than messing with DisableWhileAnim
	// which would step on the client's intention.
	// this is to avoid re-rewarding due to multi clicks.
	SetEnabled(false);
	
	if (Flashback) Flashback->ModVal(RewardFlash);

	// reward an item if possible ( the check for IsNone is to avoid return when none)
	if (!RewardItem.IsNone() && IsValid(Inventory)) {
		// return if we fail to reward
		// for example on maxed-out (e.g. picked up consumables)
		if (!Inventory->Mod(RewardItem, 1)) return;
		// given this return will cancel the effect, do before the rest.
	}
	
	// do the flags which are more flexible.
	if (IsValid(Flags)) Flags->Mod(RewardFlag, 1.0);

	// do the actor
	if (IsValid(RewardActor)) {
		RewardActor->SetActorHiddenInGame(false);
		
		ALInteract* const LReward = Cast<ALInteract>(RewardActor);
		if (LReward) LReward->Fade(true);
		else {
			// enabled separately since on begin play it checks only for AInteract and no ALInteract
			// do only if it's not an LInteract. since fade will set enable. and don't cast twice.
			AInteract* const Reward = Cast<AInteract>(RewardActor);
			if (Reward) Reward->SetEnabled(true);
		}
	}

	if (RewardStep && IsValid(Story)) Story->StartNext();

	/// rewards virtually done

	// trigger separately,
	// since sometimes the item could be rewarded outside of trigger. (e.g. manually)
	Rewarded();
	
	/// done: Process auto destroy. (do at the end.)

	if (!UseRewardDestroy) return;

	// not calling fade here, since it's called on trigger.
	// bind to destroy. or just destroy if the anim failed/finished.
	if (AnimFade->IsActive()) {
		// only bind here as we only want to destroy on reward
		AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteract::RewardFaded);
		return;
	}

	RewardFaded();
}

void ALInteract::RewardFaded() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));
	AnimFade->OnEnd.RemoveDynamic(this, &ALInteract::RewardFaded);
	Destroy();
}

bool ALInteract::TryTrigger_Implementation() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));
	// handle item req
	if (!ULockItemReq.IsNone()) {
		const bool Ok = IsValid(Inventory) && Inventory->Has(ULockItemReq);
		if (Ok) Locked = false;
	}

	// handle flag req
	if (!ULockFlagReq.IsNone()) {
		const bool Ok = IsValid(Flags) && Flags->Has(ULockFlagReq);
		if (Ok) Locked = false;
	}
	
	return Super::TryTrigger_Implementation();
}

void ALInteract::DoTrigger_Implementation() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));
	Super::DoTrigger_Implementation();

	// start fading right away to give the player the impression that they picked it up
	if (WillRewardDestroy()) Fade(false);

	bool DiagsShown = false;
	if (IsValid(Diags)) {
		// if there's a dialog. give rewards at the end of them.
		// that way we can control the story better. it's easier to check for items than for dialogs.
		Diags->OnDone.AddUniqueDynamic(this, &ALInteract::DoRewards);
		DiagsShown = Diags->AddId(TriggerDlg);
	}

	// ensure we reward or the player could get locked
	if (!DiagsShown) DoRewards();
}

void ALInteract::DoTriggerLocked_Implementation() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));
	Super::DoTriggerLocked_Implementation();
	
	if (!Inventory || !Diags) return;

	const bool Has = Inventory->Has(ULockItem);
	const FName& Dlg = Has && (!LockedItemDlg.IsNone())? LockedItemDlg : LockedDlg;
	FDialog D; FDialogChar C;
	Diags->AddId(Dlg);
}

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Item) {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));
	if (Item == NAME_None) {
		UE_LOG(LogLInteract, Warning, TEXT("%hs, TryUseItem with item none. Skip. o=%s"),
			__func__, *GetNameSafe(this));
		return EItemUseResult::BAD_TARGET;
	}

	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool ValidDiags = IsValid(Diags);

	/// Say something about it
	
	// 1st check if it's a regular item. since we don't care about lock at that stage
	// and only happens if it's specified on UseItemDlgs
	// im pretty sure this will break something else. but this needs improving later
	{
    	// check if we can say something about this
    	const FName* const pDlg = UseItemDlgs.Find(Item);
		if (pDlg) {
    		const bool Added = pDlg && ValidDiags && Diags->AddId(*pDlg);
			// assume this is not ULockItem. if you added the same item to both places then that's wrong.
			// Using bad_handled since we don't want to consume an item.
			// this is only to say something about the item.
			// if this item needs to be consumed, use ULockItem 
    		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
		}
    }

	// if it's not locked, we need not do anything with it. don't consume it.
	// there's no other functionality to TryUseItem than saying something or unlocking (implies consuming)
	if(!Locked) return EItemUseResult::BAD_TARGET;

	/// Unlock with item - at this point is locked
	
	// Checks if it needs an item to unlock it. and unlock if needed.
	const bool LockBad = Item != ULockItem;
	if (LockBad) {
		const bool Added = ValidDiags && Diags->AddId(ULockBadDlg);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	// now unlocked
	if (ValidDiags) Diags->AddId(ULockDlg);

	Locked = false; // force unlock or trigger won't work
	Trigger(); // force trigger
	return EItemUseResult::SUCCESS;
}
