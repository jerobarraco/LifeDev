#include "LInteract.h"

#include "Dialogs/Dialogs.h"
#include "Inventory/Inventory.h"

void ALInteract::BeginPlay() {
	Super::BeginPlay();
	if (!ULockItem.IsNone() || !ULockItemReq.IsNone()) {
		// note only setting it if the ulock is set.
		Locked = true;
	}

	UWorld* const World = GetWorld();
	Inventory = World->GetSubsystem<UInventory>();
	Dialogs = World->GetSubsystem<UDialogs>();
}

void ALInteract::Trigger_Implementation() {
	Super::Trigger_Implementation();
	if (ItemReward.IsNone()) return;
	if (!IsValid(Inventory)) return;
	// return maybe we maxed out
	if (!Inventory->Mod(ItemReward, 1)) return;
	if (IsValid(Dialogs)) {
		Dialogs->AddId(TriggerDlg);
	}
	
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
	Super::TryUseItem_Implementation(Name); // unnecessary actually
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
