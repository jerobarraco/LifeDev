#include "LInteract.h"

#include "Dialogs/Dialogs.h"
#include "Inventory/Inventory.h"

void ALInteract::BeginPlay() {
	Super::BeginPlay();
	if (ULockItem != NAME_None) {
		Locked = true;
	}
}

void ALInteract::Trigger_Implementation() {
	Super::Trigger_Implementation();
	if (ItemReward == NAME_None) return;
	UInventory* const Inventory = GetWorld()->GetSubsystem<UInventory>();
	
	if (!Inventory) return;
	if (!Inventory->Mod(ItemReward, 1)) return;
	Destroy();
	// TODO trigger sfx? i cannot because i just destroyed myself
}

// TODO maybe have 2 more sounds for locked and unlocked
void ALInteract::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	UInventory* const Inventory = GetWorld()->GetSubsystem<UInventory>();
	UDialogs* const Dialogs = GetWorld()->GetSubsystem<UDialogs>();
	if (!Inventory || !Dialogs) return;
	const bool Has = Inventory->Has(ULockItem);
	const FName& Dlg = Has && (LockItemDlg != NAME_None) ? LockItemDlg : LockDlg;
	FDialog D; FDialogChar C;
	Dialogs->AddId(Dlg);
	PlaySFX(SFX_Locked);
}

EItemUseResult ALInteract::TryUseItem_Implementation(const FName& Name) {
	Super::TryUseItem_Implementation(Name); // unnecessary actually
	const bool Ok = Name == ULockItem;
	if (Ok) {
		UDialogs* const Dialogs = GetWorld()->GetSubsystem<UDialogs>();
		if (Dialogs && ULockDlg != NAME_None) {
			Dialogs->AddId(ULockDlg);
		}
		// force unlock or trigger won't work
		Locked = false;
		// force trigger
		Trigger();
	}

	return Ok ? EItemUseResult::SUCCESS: EItemUseResult::BAD_TARGET;
}
