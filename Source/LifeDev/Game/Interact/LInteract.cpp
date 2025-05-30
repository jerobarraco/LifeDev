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

#include "LifeDev/Core/Sentry.h"
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
		Diags->AddId(DName);
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

void ALInteract::SetActive_Implementation(const bool Active) {
	// skip activating if is UseActiveOnce and has been activated
	// doing inside SetActive to capture all attempts of activation.
	// including the steps and other interactions.
	// These two are the most important since they are triggered automatically. Hence, the code needs the check here.
	if (UNLIKELY(Active & UseActiveOnce)) { // todo change UNLIKELY if its used more
		const FName NTrigger(LDConsts::Dlgs::Inter::TriggerPre+Label.ToString());
		if (UNLIKELY(Flags && Flags->Has(NTrigger))) {
			UE_LOG(LogLInteract, Log, TEXT("%hs not activating. UseActivateOnce && FlagHas. N=%s"),
				__func__, *NTrigger.ToString());
			return;
		}
		UE_CLOG(UNLIKELY(!Flags), LogLInteract, Warning, TEXT("%hs Flags subsystem is not loaded!. N=%s"),
			__func__, *NTrigger.ToString());
	}

	Super::SetActive_Implementation(Active);
}

bool ALInteract::TryTrigger_Implementation() {
	const USentry* const Sentry = USentry::Instance(this);
	// keep an eye on this in case it slows us down.
	if (LIKELY(Sentry)) Sentry->AddHint(LDConsts::Dlgs::Inter::TriggerPre+Label.ToString(), {});

	return Super::TryTrigger_Implementation();
}

void ALInteract::Fade_Implementation(const bool FadeIn, const bool SetHidden) {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s in=%i hidden=%i useFade=%i"),
		__func__, *Label.ToString(), FadeIn, SetHidden, UseFade);

	// before the fade on purpose. for the hidden and the bind
	if (SetHidden) {
		if (FadeIn | !UseFade)
			SetActorHiddenInGame(!FadeIn); // handle hidden if no UseFade is set
		else // !FadeIn && UseFade
			AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteract::HideAfterFade);
	} else
		// i'm not super sure about this.
		// probably could collide with the intention of fading something in without being enabled
		// might happen on a step auto-fading something.
		// only on !SetHidden since SetActorHidden also handles setActive
		// please me from the future, be careful. "ki o tsukete!"
		if (!FadeIn | UseAutoActivate) SetActive(FadeIn); //!FadeIn important to not have the collision while faded

	if (UseFade) {
		AnimFade->IsReversed = FadeIn;
		AnimFade->Activate(true);
	}
}

void ALInteract::BeginPlay() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	// these should be loaded first. since super::BeginPlay will call SetActive, which needs the flags.
	Inventory = World->GetSubsystem<UInventory>();
	Diags = World->GetSubsystem<UDiags>();
	Flags = World->GetSubsystem<UFlags>();
	Flashback = World->GetSubsystem<UFlashback>();
	Story = World->GetSubsystem<UStory>();

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

	// hide and disable reward actor if any. will also call SetActive.
	AActor* const RAct = RewardActor.Get();
	if (IsValid(RAct)) RAct->SetActorHiddenInGame(true);
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
	if (LIKELY(Flags)) Flags->Mod(RewardFlag, 1.0);

	// do the actor
	AActor* const RAct = RewardActor.Get();
	if (IsValid(RAct)) {
		// fade if possible, otherwise unset hidden. setactive follows.
		ALInteract* const LReward = Cast<ALInteract>(RAct);
		if (LReward) LReward->Fade(true);
		else RewardActor->SetActorHiddenInGame(false);
	}

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

	if (UNLIKELY(!IsValid(Inventory))) return false; // false because ulockitemreq is not none here

	// consume items when unlocking
	UnlockItems.Add(ULockItemReq); // TODO remove ulockitemreq
	// handle unlockitems
	const FString& Base = LDConsts::Dlgs::Inter::UseItemPre + Label.ToString();
	for (int32 i = UnlockItems.Num()-1; i>=0; --i) {
		const FName& N = UnlockItems[i];
		// returns true on consumables, and true on non-consumables that i have
		// the !N.IsNone() will allow to remove none items and check for empty array
		if (!N.IsNone() && !Inventory->Mod(N, -1, true)) continue;

		// trigger the dialog here. avoid extra if below
		const bool Added = Diags->AddId(FName(Base+"."+N.ToString()));
		UnlockItems.RemoveAtSwap(i); // do here to avoid a crash in N.ToString() since it's a ref. and i rather not copy it. though it probably is the size of a pointer anyway.

		// when we've removed all, unlock.
		// notice this point is only achieved if .Num() > 0 to begin with 
		if (UnlockItems.Num() == 0) {
			if (!Added) Diags->AddId(FName(Base)); // nopes because it will trigger for each item
			return true;
		}
	}

	// handle ulockitemreq
	if (ULockItemReq.IsNone()) return false; // false because no need to call unlock.
	FItem Item;
	if (!Inventory->Get(ULockItemReq, Item)) return false; // false because we don't have it

	return Inventory->IsCold(Item);
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
		DiagsShown = DiagsShown | AutoShown;
	}

	// ensure we reward or the player could get locked
	if (!DiagsShown) DoRewards();
}

void ALInteract::DoTriggerLocked_Implementation() {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
	Super::DoTriggerLocked_Implementation();

	if (UNLIKELY(!Inventory | !Diags)) return;

	const bool Has = Inventory->Has(ULockItem);

	const FName& Dlg = (Has & !LockedItemDlg.IsNone())? LockedItemDlg : LockedDlg;
	Diags->AddId(Dlg);

	// Also add the auto ones
	const FString& SLabel = Label.ToString();
	const FName TName = FName(LDConsts::Dlgs::Inter::TriggerLPre+SLabel);
	Diags->AddId(TName);
	if (LIKELY(Flags)) Flags->Mod(TName, 1);
}

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Item) {
	UE_LOG(LogLInteract, Log, TEXT("%hs o=%s"), __func__, *Label.ToString());
#if !LD_ITEM_USE
	UE_LOG(LogLInteract, Warning, TEXT("%hs Item usage disabled! o=%s"), __func__, *Label.ToString());
	return EItemUseResult::BAD_TARGET;
#else
	if (UNLIKELY(Item.IsNone())) {
		UE_LOG(LogLInteract, Warning, TEXT("%hs, TryUseItem with item none. Skip. o=%s"),
			__func__, *Label.ToString());
		return EItemUseResult::BAD_TARGET;
	}

	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool ValidDiags = IsValid(Diags);

	/// Say something about it

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
#endif
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
