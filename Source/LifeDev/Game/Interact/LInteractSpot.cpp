#include "LInteractSpot.h"

#include "Dialogs/Dialogs.h"
#include "Interact/CInteract.h"
#include "Inventory/Inventory.h"

ALInteractSpot::ALInteractSpot():Super() {
	// ItemSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawn"));
	// ItemSpawnPos->SetupAttachment(IRoot);
	// always locked. we don't want it to trigger cuz that gives the reward.
	// it will trigger automatically
	Locked = true;
}

void ALInteractSpot::BeginPlay() {
	Super::BeginPlay();

	Inventory = GetWorld()->GetSubsystem<UInventory>();
	Dialogs = GetWorld()->GetSubsystem<UDialogs>();
}

void ALInteractSpot::Trigger_Implementation() {
	Super::Trigger_Implementation();

	// reward here to allow to be overriden
	// todo make this into a function in a new intermediary class
	if (!Items.IsEmpty()) return; // don't trigger if we don't have all the items.

	if (IsValid(Dialogs)) {
		Dialogs->AddId(TriggerDlg);
	}
	
	if (ItemReward.IsNone()) return;
	if (!IsValid(Inventory)) return;
	if (!Inventory->Mod(ItemReward, 1)) return;
}

void ALInteractSpot::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	if (!Inventory || !Dialogs) return;
	Dialogs->AddId(TriggerBadDlg);
}

EItemUseResult ALInteractSpot::TryUseItem_Implementation(const FName& Name) {
	Super::TryUseItem_Implementation(Name); // unnecessary actually
	if (Items.IsEmpty()) {
		Locked = true; // unnecessary but meh
		const bool Added = IsValid(Dialogs) && Dialogs->AddId(FullDlg);
		return Added ?  EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	int32 Id;
	const bool Ok = Items.Find(Name, Id);
	EItemUseResult Result = Ok ? EItemUseResult::SUCCESS : EItemUseResult::BAD_TARGET;
	if (Ok) {
		if (IsValid(Dialogs)) {
			Dialogs->AddId(DropDlg);
		}
		Items.RemoveAt(Id);
		if (Items.IsEmpty()) {
			Locked = false; // allow to trigger
			Trigger(); // force trigger on all items restored
			Locked = true; // avoid further triggering
		}
	} else {
		const bool Added = IsValid(Dialogs) && Dialogs->AddId(DropBadDlg);
		Result = Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

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
