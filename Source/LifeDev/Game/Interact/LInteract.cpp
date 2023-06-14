#include "LInteract.h"

#include "Dialogs/Dialogs.h"
#include "Inventory/Inventory.h"

void ALInteract::BeginPlay() {
	Super::BeginPlay();
	if (!ULockItem.IsNone()) {
		// note only setting it if the ulock is set.
		Locked = true;
	}
	
	Inventory = GetWorld()->GetSubsystem<UInventory>();
	Dialogs = GetWorld()->GetSubsystem<UDialogs>();
}

void ALInteract::Trigger_Implementation() {
	Super::Trigger_Implementation();
	if (ItemReward.IsNone()) return;
	if (!IsValid(Inventory)) return;
	if (!Inventory->Mod(ItemReward, 1)) return;
	if (IsValid(Dialogs) && !TriggerDlg.IsNone()) {
		Dialogs->AddId(TriggerDlg);
	}
	
	Destroy();
	// TODO trigger sfx? i cannot because i just destroyed myself
}

// TODO maybe have 2 more sounds for locked and unlocked
void ALInteract::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	if (!Inventory || !Dialogs) return;
	const bool Has = Inventory->Has(ULockItem);
	const FName& Dlg = Has && (!LockItemDlg.IsNone())? LockItemDlg : LockDlg;
	FDialog D; FDialogChar C;
	Dialogs->AddId(Dlg);
	PlaySFX(SFX_Locked);
}

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Name) {
	Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool Ok = Name == ULockItem;
	EItemUseResult Result = Ok ? EItemUseResult::SUCCESS: EItemUseResult::BAD_TARGET;
	if (Ok) {
		if (Dialogs && !ULockDlg.IsNone()) {
			Dialogs->AddId(ULockDlg);
			Result = EItemUseResult::BAD_HANDLED;
		}
		// force unlock or trigger won't work
		Locked = false;
		// force trigger
		Trigger();
	}

	return Result;
}
