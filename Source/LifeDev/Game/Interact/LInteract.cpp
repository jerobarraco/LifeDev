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
	if (!ULockItem.IsNone() || !ULockItemReq.IsNone() || !ULockFlagReq.IsNone()) {
		// note only setting it if the ulock is set.
		Locked = true;
	}

	if (UseAnimFade) {
		AnimFade->SetMaterial();
	} else {
		AnimFade->SetActive(false);
		AnimFade->Meshes.Empty();
	}

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	Inventory = World->GetSubsystem<UInventory>();
	Dialogs = World->GetSubsystem<UDiags>();
	Flags = World->GetSubsystem<UFlags>();
}

void ALInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	AnimFade->OnEnd.RemoveAll(this);

	Inventory = nullptr;
	Dialogs = nullptr;
	Flags = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALInteract::Trigger_Implementation() {
	Super::Trigger_Implementation();

	// trigger the dialog anyway.
	if (IsValid(Dialogs)) {
		Dialogs->AddId(TriggerDlg);
	}
	
	UWorld* const World = GetWorld();
	if (!World) return;
	
	if (!FMath::IsNearlyZero(TriggerFlashInc)) {
		UFlashback* const Flashback = World->GetSubsystem<UFlashback>();
		if (Flashback) {
			Flashback->ModVal(TriggerFlashInc);
		}
	}

	if (IsValid(Flags)) {
		Flags->Mod(FlagReward, 1.0);
	}
	
	// reward an item if possible
	if (ItemReward.IsNone()) return;
	if (!IsValid(Inventory)) return;
	// return if we maxed out
	if (!Inventory->Mod(ItemReward, 1)) return;

	// avoid re-rewarding due to multi clicks
	SetEnabled(false);

	ItemRewarded();
	// Process auto destroy
	if (UseAnimFade) {
		// only bind here as we only want to destroy on reward
		AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteract::RewardFaded);
		Fade(false);
	} else {
		RewardFaded(); // this can destroy the object. don't do anything after this.
	}
}

void ALInteract::ItemRewarded_Implementation() {}

void ALInteract::RewardFaded() {
	AnimFade->OnEnd.RemoveDynamic(this, &ALInteract::RewardFaded);
	if (!AutoDestroy) return;
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

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Name) {
	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool Ok = !ULockItem.IsNone() && Name == ULockItem;
	if (!Ok) {
		const bool Added = IsValid(Dialogs) && Dialogs->AddId(ULockBadDlg);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	} 

	// at this point is ok.
	if (IsValid(Dialogs)) {
		Dialogs->AddId(ULockDlg);
	}
	
	Locked = false; // force unlock or trigger won't work
	Trigger(); // force trigger
	return EItemUseResult::SUCCESS;
}
