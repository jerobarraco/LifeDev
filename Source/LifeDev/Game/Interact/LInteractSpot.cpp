#include "LInteractSpot.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"

ALInteractSpot::ALInteractSpot():Super() {
	// always locked. we don't want it to trigger because that gives the reward.
	// it will trigger automatically
	UseAnim = false;
	Locked = true;
	UseRewardDestroy = false;
	UseFade = false;
	UseStateLoop = false;
	StateNum = 1;
	Texts = {
		// FText::FromString(TEXT("Drop here")),
		FText::FromString(TEXT("Full"))
	};
	Super::SetMobility(EComponentMobility::Static);
}

EItemUseResult ALInteractSpot::TryUseItem_Implementation(const FName& Name) {
	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	if (UNLIKELY(Items.IsEmpty())) {
		Locked = true; // unnecessary but complete, jic
		const bool Added = LIKELY(IsValid(Diags)) && Diags->AddId(DropFullDlg);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	int32 Id;
	const bool Ok = Items.Find(Name, Id);
	if (!Ok) {
		const bool Added = LIKELY(IsValid(Diags)) && Diags->AddId(DropBadDlg);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	if (UseOrder && Id !=0) { // TODO test
		const bool Added = LIKELY(IsValid(Diags)) && Diags->AddId(DropBadOrderDlg);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	if (LIKELY(IsValid(Diags))) Diags->AddId(DropDlg);

	Items.RemoveAtSwap(Id);
	if (UNLIKELY(Items.IsEmpty())) {
		Locked = false; // allow to trigger
		Trigger(); // force trigger on all items restored
		Locked = true; // avoid further triggering
		LockedDlg = LockedFullDlg; // from now on use the new dialog
	}

	return EItemUseResult::SUCCESS;
}
