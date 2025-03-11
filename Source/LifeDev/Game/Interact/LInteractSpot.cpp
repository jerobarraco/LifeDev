#include "LInteractSpot.h"

#include "Diags/Diags.h"

ALInteractSpot::ALInteractSpot():Super() {
	// always locked. we don't want it to trigger because that gives the reward.
	// it will trigger automatically
	Locked = true;
	IsOneShot = true; // disable after use
	UseAnim = false;
	UseRewardDestroy = false;
	UseFade = false;
	UseOrder = false;
	UseStateLoop = false; // lock on the last state, since it might trigger more time than the num of states.
	StateNum = 2; // open and done
	Texts = {
		FText::FromString(TEXT("Drop here")),
		FText::FromString(TEXT("Full"))
	};
	Super::SetMobility(EComponentMobility::Static);
}

EItemUseResult ALInteractSpot::TryUseItem_Implementation(const FName& Name) {
	// Super::TryUseItem_Implementation(Name); // unnecessary actually
	// TODO move auto dialogs to the consts

	const FString& SLabel = Label.ToString();
	if (UNLIKELY(Items.IsEmpty())) {
		Locked = true; // unnecessary but complete, jic
		const bool Added = LIKELY(IsValid(Diags)) &&
			Diags->AddId(FName("Inter.Spot.Use.Full."+SLabel));
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	int32 Id;
	const bool Ok = Items.Find(Name, Id);
	if (!Ok) {
		const bool Added = LIKELY(IsValid(Diags)) &&
			Diags->AddId(FName("Inter.Spot.Use.Bad."+SLabel));
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	if (UseOrder && Id !=0) { // TODO test
		const bool Added = LIKELY(IsValid(Diags)) &&
			Diags->AddId(FName("Inter.Spot.Use.BadOrder."+SLabel));
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	if (LIKELY(IsValid(Diags)))
		Diags->AddId(DropDlg) ||
		Diags->AddId(FName("Inter.Spot.Use."+SLabel));

	Items.RemoveAtSwap(Id);
	if (UNLIKELY(Items.IsEmpty())) {
		Locked = false; // allow to trigger
		Trigger(); // force trigger on all items restored
		Locked = true; // avoid further triggering
		LockedDlg = LockedFullDlg; // from now on use the new dialog
	} else if (UseStateInc) { // on the else since Trigger always increases state
		SetState(State+1);
	}

	return EItemUseResult::SUCCESS;
}
