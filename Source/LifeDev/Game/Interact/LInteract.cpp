#include "LInteract.h"

#include "Components/AudioComponent.h"
#include "Dialogs/Dialogs.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALInteract::ALInteract():Super() {
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAtt(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/SA_GenericAttenuation.SA_GenericAttenuation"));
	SFX->AttenuationSettings = CAtt.Object; 
}

void ALInteract::BeginPlay() {
	Super::BeginPlay();
	if (!ULockItem.IsNone() || !ULockItemReq.IsNone()) {
		// note only setting it if the ulock is set.
		Locked = true;
	}

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	Inventory = World->GetSubsystem<UInventory>();
	Dialogs = World->GetSubsystem<UDialogs>();
}

void ALInteract::Trigger_Implementation() {
	Super::Trigger_Implementation();

	// trigger the dialog anyway.
	if (!TriggerDlg.IsNone() && IsValid(Dialogs)) {
		Dialogs->AddId(TriggerDlg);
	}

	if (!FMath::IsNearlyZero(TriggerFlashInc)) {
		GetWorld()->GetSubsystem<UFlashback>()->IncVal(TriggerFlashInc);
	}

	// reward an item if possible
	if (ItemReward.IsNone()) return;
	if (!IsValid(Inventory)) return;
	// return if we maxed out
	if (!Inventory->Mod(ItemReward, 1)) return;

	ItemRewarded();
}

void ALInteract::ItemRewarded_Implementation() {
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
		// check if we have the item
		bool Ok = IsValid(Inventory) && Inventory->Has(ULockItemReq);
		// unlock if no item is needed to unlock
		if (Ok && ULockItem.IsNone()) {
			Locked = false;
		}
	}

	return Super::TryTrigger_Implementation();
}

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Name) {
	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool Ok = !ULockItem.IsNone() && Name == ULockItem;
	EItemUseResult Result = Ok ? EItemUseResult::SUCCESS: EItemUseResult::BAD_TARGET;
	if (Ok) {
		if (IsValid(Dialogs)) {
			Dialogs->AddId(ULockDlg);
		}
		// force unlock or trigger won't work
		Locked = false;
		// force trigger
		Trigger();
	} else {
		const bool Added = IsValid(Dialogs) && Dialogs->AddId(ULockBadDlg);
		Result = Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	return Result;
}
