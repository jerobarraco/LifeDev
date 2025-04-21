#include "LInteract.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundClass.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "Story/Story.h"
#include "CQuickMesh.h" // this is necessary for the .add(Mesh) below. rider says it's not but don't believe him. windows will fail.

#include "LifeDev/Core/Consts/ConstDlgs.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"

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

	// set to true, it's needed for the range. TODO if it creates issues rollback to false.
	// added here to not clutter the CInteract base.
	Interact->SetGenerateOverlapEvents(true);
}

void ALInteract::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	if (LIKELY(IsValid(Diags))) {
		const FString& BName = LDConsts::Dlgs::Inter::StatePre;
		const FString& SLabel = Label.ToString();
		const FName DName(BName+SLabel+"."+FString::FromInt(NewState));
		Diags->AddId(DName); // todo don't warn?
	}
	/*
	not sure about this. it will allow to use it in flagsFinish, unlockCondition, dialogs, and other stuff.
	it might even help restore the state of a level on load (hmmmm).
	but it might also spam the flags with a ton of useless stuff. and also has the problem of setstateNow,
	which, again, will trigger side effects.
	also i already have a way to check the state of an interact via the conditions.
	const FString& BName = LDConsts::Dlgs::Inter::StatePre;
	const FString& SLabel = Label.ToString();
	if (LIKELY(IsValid(Flags)) Flags->Set(FName(BName+SLabel), NewState)
	*/
}

void ALInteract::Fade_Implementation(const bool FadeIn, const bool SetHidden) {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s in=%i hidden=%i useFade=%i"),
		__func__, *Label.ToString(), FadeIn, SetHidden, UseFade);
	// i'm not super sure about this.
	// probably could collide with the intention of fading something in without being enabled
	// might happen on a step auto-fading something.
	// please me from the future, be careful. "ki o tsukete!"
	SetActive(FadeIn);

	// before the fade on purpose. for the hidden and the bind
	if (SetHidden) {
		if (FadeIn || !UseFade)
			SetActorHiddenInGame(!FadeIn); // handle hidden if no UseFade is set
		else // !FadeIn && UseFade
			AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteract::HideAfterFade);
	}
	
	if (UseFade) {
		AnimFade->IsReversed = FadeIn;
		AnimFade->Activate(true);
	}
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

	if (WillRewardDestroy()) {
		// avoid getting the sound killed on self-destroy
		// set here on purpose to allow the user to override it and self-hurt.
		UseAttachedSFX = false;
		UE_CLOG(!UseFade, LogLInteract, Warning, TEXT("%hs Will RewardDestroy but UseFade is false. "
			"This is legal but unlikely. o=%s"), __func__, *Label.ToString());
	}

	if (IsValid(RewardActor)) { // hide and disable reward actor if any.
		RewardActor->SetActorHiddenInGame(true);
		AInteract* const Reward = Cast<AInteract>(RewardActor);
		if (IsValid(Reward)) Reward->SetActive(false);
	}

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	Inventory = World->GetSubsystem<UInventory>();
	Diags = World->GetSubsystem<UDiags>();
	Flags = World->GetSubsystem<UFlags>();
	Flashback = World->GetSubsystem<UFlashback>();
	Story = World->GetSubsystem<UStory>();

	if (UNLIKELY(UseActiveOnce)) { // todo change UNLIKELY if its used more
		// intentionally not force-enabling, to not step to other custom code.
		const FName NTrigger(LDConsts::Dlgs::Inter::TriggerPre+Label.ToString());
		if (UNLIKELY(Flags->Has(NTrigger))) SetActive(false);
	}
}

void ALInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	AnimFade->OnEnd.RemoveAll(this);

	Inventory = nullptr;
	Diags = nullptr;
	Flags = nullptr;
	Flashback = nullptr;
	Story = nullptr;
	RewardActor = nullptr; // free reference
	
	Super::EndPlay(EndPlayReason);
}

void ALInteract::DoRewards() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
	Diags->OnDone.RemoveDynamic(this, &ALInteract::DoRewards);

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	/// Rewards
	// just return. nothing to do. don't self destroy or anything.
	if (IsRewardless()) {
		UE_LOG(LogLInteract, Log, TEXT("%hs Nothing to reward. Skip. o=%s"),
			__func__, *Label.ToString());
		return;
	}

	// reward an item if possible ( the check for IsNone is to avoid return when none)
	if (!RewardItem.IsNone()) {
		// return if we fail to reward
		// for example on maxed-out (e.g. picked up consumables)
		if (UNLIKELY(!IsValid(Inventory) || !Inventory->Mod(RewardItem, 1))) {
			Fade(true); // faded before calling doRewards. fade will call setactive which is also needed.
			return;
			// given this return will cancel the effect, do before the rest.
		}
	}
	
	if (LIKELY(Flashback)) Flashback->ModVal(RewardFlash);
	// do the flags which are more flexible.
	if (LIKELY(IsValid(Flags))) Flags->Mod(RewardFlag, 1.0);

	// do the actor
	if (IsValid(RewardActor)) {
		RewardActor->SetActorHiddenInGame(false);
		
		ALInteract* const LReward = Cast<ALInteract>(RewardActor);
		if (LReward) LReward->Fade(true);
		else {
			// enabled separately since on begin play it checks only for AInteract and no ALInteract
			// do only if it's not an LInteract. since fade will set enable. and don't cast twice.
			AInteract* const Reward = Cast<AInteract>(RewardActor);
			if (Reward) Reward->SetActive(true);
		}
	}

	if (RewardStep && IsValid(Story)) Story->StartNext();

	/// rewards virtually done

	// trigger separately,
	Rewarded();
	
	/// done: Process auto destroy. (do at the end.)

	if (!UseRewardDestroy) return;

	// not calling fade here, since it's called on trigger.
	// bind to destroy. or just destroy if the anim failed/finished.
	if (AnimFade->IsActive()) {
		// only bind here as we only want to destroy on reward
		AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteract::DestroyAfterReward);
		return;
	}

	DestroyAfterReward();
}

void ALInteract::DestroyAfterReward() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
	AnimFade->OnEnd.RemoveDynamic(this, &ALInteract::DestroyAfterReward);
	Destroy();
}

void ALInteract::HideAfterFade() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
	AnimFade->OnEnd.RemoveDynamic(this, &ALInteract::HideAfterFade);
	SetActorHiddenInGame(true);
}

bool ALInteract::ShouldUnlock_Implementation() {
	if (Super::ShouldUnlock_Implementation()) return true; // it's enough if it passes on parent already

	// handle item req
	if (ULockItemReq.IsNone()) return false;

	return LIKELY(IsValid(Inventory)) && Inventory->Has(ULockItemReq);
}

void ALInteract::Unlock_Implementation() {
	if (UNLIKELY(!IsLocked)) return; // avoid triggering the dialogs again

	Super::Unlock_Implementation();
	const FString& SLabel = Label.ToString();
	const FName Dlg(LDConsts::Dlgs::Inter::UnlockPre+SLabel);
	// now unlocked
	if (LIKELY(IsValid(Diags))) Diags->AddId(Dlg); // add before flags to avoid race conditions (flags have side-effects)

	// also flags since sometimes the diag might not exist
	if (LIKELY(Flags)) Flags->Mod(Dlg, 1);
}

void ALInteract::DoTrigger_Implementation() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
	// force disablewhileAnim when there's a reward. so it deactivates.
	// in the hope of that avoiding issues of quick clicks triggering multiple times.
	// before Super since super will do the setactive stuff.
	// this way it won't call SetActive multiple times either (a plus)
	// also DoRewards might be called with a delay from the dialog anyway, which is undesireable.
	if (!IsRewardless()) DisableWhileAnim = true;

	Super::DoTrigger_Implementation();
	const FName TName = FName(LDConsts::Dlgs::Inter::TriggerPre+Label.ToString());
	if (LIKELY(Flags)) {
		Flags->Mod(LDConsts::Flags::Stats::Inter::Trigger, 1);
		Flags->Mod(TName, 1);
	}

	// start fading right away to give the player the impression that they picked it up
	if (WillRewardDestroy()) Fade(false);

	bool DiagsShown = false; // using this instead of a return to capture TriggerDlg being none, AND being invalid.
	if (LIKELY(IsValid(Diags))) {
		// if there's a dialog. give rewards at the end of them.
		// that way we can control the story better. it's easier to check for items than for dialogs.
		Diags->OnDone.AddUniqueDynamic(this, &ALInteract::DoRewards);
		// Keep using the stock TriggerDlg &Co. they are superior. and i don't want to over-rely on a new system.
		// still call it if the stock one fails.
		DiagsShown = Diags->AddId(TriggerDlg);
		// done this way to force the autodialog even if the class one is defined 
		const bool AutoShown = Diags->AddId(TName);
		DiagsShown = DiagsShown || AutoShown;
	}

	// ensure we reward or the player could get locked
	if (!DiagsShown) DoRewards();
}

void ALInteract::DoTriggerLocked_Implementation() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
	Super::DoTriggerLocked_Implementation();
	
	if (UNLIKELY(!Inventory || !Diags)) return;

	const bool Has = Inventory->Has(ULockItem);

	const FName& Dlg = (Has && !LockedItemDlg.IsNone())? LockedItemDlg : LockedDlg;
	Diags->AddId(Dlg);

	// Also add the auto ones
	const FString& SLabel = Label.ToString();
	const FName TName = FName(LDConsts::Dlgs::Inter::TriggerLPre+SLabel);
	Diags->AddId(TName);
	if (LIKELY(Flags)) Flags->Mod(TName, 1);
}

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Item) {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
	if (UNLIKELY(Item.IsNone())) {
		UE_LOG(LogLInteract, Warning, TEXT("%hs, TryUseItem with item none. Skip. o=%s"),
			__func__, *Label.ToString());
		return EItemUseResult::BAD_TARGET;
	}

	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool ValidDiags = IsValid(Diags);

	/// Say something about it
	
	// 1st check if it's a regular item. since we don't care about lock at that stage
	// and only happens if it's specified on UseItemDlgs
	// i'm pretty sure this will break something else. but this needs improving later
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

	const FString& SLabel = Label.ToString();
	// generic say something when using an item. deprecated UseItemDlgs
	if (LIKELY(ValidDiags))
		Diags->AddId(FName(LDConsts::Dlgs::Inter::UseItemPre+SLabel+"."+Item.ToString()));

	// if it's not locked, we need not do anything with it. don't consume it.
	// there's no other functionality to TryUseItem than saying something or unlocking (implies consuming)
	if(!IsLocked) return EItemUseResult::BAD_TARGET;

	/// Unlock with item - at this point is locked
	
	// Checks if it needs an item to unlock it. and unlock if needed.
	const bool LockBad = Item != ULockItem;
	if (LockBad) {
		const FName Dlg(LDConsts::Dlgs::Inter::UnlockBadPre + SLabel);
		const bool Added = LIKELY(ValidDiags) && Diags->AddId(Dlg); // add before flag. to avoid race conditions.
		if (LIKELY(Flags)) Flags->Mod(Dlg, 1); // also as a flag

		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	Unlock();
	Trigger(); // force trigger
	return EItemUseResult::SUCCESS;
}

// too much spam. not needed atm. iranai. muda da.
// void ALInteract::SetActive_Implementation(const bool Active) {
// 	Super::SetActive_Implementation(Active);
//
// 	if (LIKELY(IsValid(Diags))) { // TODO test
// 		const FString& DlgPre = Active ?
// 				LDConsts::Dlgs::Inter::ActivatePre:
// 				LDConsts::Dlgs::Inter::DeactivatePre;
// 		const FName DName(DlgPre+GetActorLabel(false));
// 		Diags->AddId(DName);  // todo don't warn?
// 	}
// }
