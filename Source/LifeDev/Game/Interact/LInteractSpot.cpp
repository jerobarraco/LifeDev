#include "LInteractSpot.h"

#include "Dialogs/Dialogs.h"
#include "Interact/CInteract.h"
#include "Inventory/Inventory.h"

// ALInteractSpot::ALInteractSpot():Super() {
	// ItemSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawn"));
	// ItemSpawnPos->SetupAttachment(IRoot);
// }

void ALInteractSpot::BeginPlay() {
	Super::BeginPlay();

	Inventory = GetWorld()->GetSubsystem<UInventory>();
	Dialogs = GetWorld()->GetSubsystem<UDialogs>();
}

void ALInteractSpot::Trigger_Implementation() {
	Super::Trigger_Implementation();

	// reward here to allow to be overriden
	// todo make this into a function in a new intermediary class
	if (ItemReward.IsNone()) return;
	if (!IsValid(Inventory)) return;
	if (!Inventory->Mod(ItemReward, 1)) return;
	if (IsValid(Dialogs) && !TriggerDlg.IsNone()) {
		Dialogs->AddId(TriggerDlg);
	}

	// if (!ItemSpawn.IsNone()) {
		// FItem Item;
		// Inventory->Get(ItemSpawn, Item);
		// FActorSpawnParameters Params;
		// Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// TODO add scene component to define the transform
		// GetWorld()->SpawnActor(Item.Obj, &ItemSpawnPos->GetComponentTransform() , Params);
	// }
	// Destroy();
	// TODO trigger sfx? 
}

void ALInteractSpot::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	if (!Inventory || !Dialogs) return;
	Dialogs->AddId(FullDlg);

	// const bool Has = Inventory->Has(ULockItem);
	// const FName& Dlg = Has && (!LockItemDlg.IsNone())? LockItemDlg : LockDlg;
	// FDialog D; FDialogChar C;
	// Dialogs->AddId(Dlg);
	// PlaySFX(SFX_Locked);
}

EItemUseResult ALInteractSpot::TryUseItem_Implementation(const FName& Name) {
	Super::TryUseItem_Implementation(Name); // unnecessary actually
	if (Items.IsEmpty()) {
		Locked = true;
		if (FullDlg.IsNone() || !IsValid(Dialogs)) {
			return EItemUseResult::BAD_TARGET;
		}
		Dialogs->AddId(FullDlg);
		return EItemUseResult::BAD_HANDLED;
	}

	int32 Id;
	const bool Ok = Items.Find(Name, Id);
	if (Ok) {
		if (Dialogs && !CorrectDlg.IsNone()) {
			Dialogs->AddId(CorrectDlg);
		}
		Items.RemoveAt(Id);
	}

	if (Items.IsEmpty()) {
		Locked = false;
		Trigger(); // force trigger on all items restored
		Locked = true; // avoid further triggering
	}

	const EItemUseResult Result = Ok ? EItemUseResult::SUCCESS: EItemUseResult::BAD_TARGET;
	return Result;
}

void ALInteractSpot::SetText_Implementation() {
	Super::SetText_Implementation();
	int32 Num = Texts.Num();
	int32 I = Num == 0 ? -1 : (Num == 1 ? 0 : (Items.IsEmpty()? 1: 0));
	if (I<0) {
		return;
	}
	Interact->Text = Texts[I];
}
