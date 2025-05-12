#include "LInteractSpot.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"

#include "LifeDev/Core/Consts/ConstDlgs.h"

ALInteractSpot::ALInteractSpot():Super() {
	// always locked. we don't want it to trigger because that gives the reward.
	// it will trigger automatically
	IsLocked = true;
	IsOneShot = true; // disable after use
	UseAnim = false;
	UseRewardDestroy = false; // spots always reward stuff. but don't disappear.
	UseFade = false;
	UseOrder = false;
	UseStateLoop = false; // lock on the last state, since it might trigger more time than the num of states.
	StateNum = 2; // open and done
	Texts = {
		NSLOCTEXT("LSpot", "State0", "Drop Here"),
		NSLOCTEXT("LSpot", "State0", "Full"),
	};
	Super::SetMobility(EComponentMobility::Static); // spots don't need to move, by default.
}

EItemUseResult ALInteractSpot::TryUseItem_Implementation(const FName& Name) {
	// Super::TryUseItem_Implementation(Name); // unnecessary actually
#if !LD_ITEM_USE
	UE_LOG(LogTemp, Warning, TEXT("%hs Attempted usage on spot o=%s i=%s"),
		__func__, *Label.ToString(), *Name.ToString());
	if (LIKELY(true)) return EItemUseResult::BAD_TARGET; // if(true) avoid complaining about unreachable
#endif
	const FString& SLabel = Label.ToString();
	if (UNLIKELY(Items.IsEmpty())) {
		IsLocked = true; // unnecessary but complete, jic
		const FName Row(LDConsts::Dlgs::Inter::Spot::FullPre+SLabel);
		const bool Added = LIKELY(IsValid(Diags)) && Diags->AddId(Row);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	int32 Id;
	const bool Ok = Items.Find(Name, Id);
	if (!Ok) {
		const FName Row(LDConsts::Dlgs::Inter::Spot::BadPre+SLabel);
		const bool Added = LIKELY(IsValid(Diags)) && Diags->AddId(Row);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	if (UseOrder && Id !=0) { // TODO test
		const FName Row(LDConsts::Dlgs::Inter::Spot::BadOrderPre+SLabel);
		const bool Added = LIKELY(IsValid(Diags)) && Diags->AddId(Row);
		return Added ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	if (LIKELY(IsValid(Diags)))
		Diags->AddId(DropDlg) ||
		Diags->AddId(FName(LDConsts::Dlgs::Inter::Spot::DropPre+SLabel)); // TODO test

	Items.RemoveAtSwap(Id);
	if (UNLIKELY(Items.IsEmpty())) {
		TriggerForced();
	} else if (UseStateInc) { // on the else since Trigger always increases state
		SetState(State+1);
	}

	// todo since trigger is called before return, then Item.Use is done before Trigger. so the dialogs are in reverse.
	return EItemUseResult::SUCCESS;
}

bool ALInteractSpot::TryTrigger_Implementation() {
#if LD_ITEM_USE
	return Super::TryTrigger_Implementation();
#else
	// somewhat similar to should unlock. which is not great.
	// consume items when unlocking

	bool Used = false;
	for (int32 i = Items.Num()-1; i>=0; --i) {
		const FName& N = Items[i];
		FItem Item;
		Used = Inventory->Get(N, Item); 
		if (!Used) continue; // not return, so that it can consume the rest, otherwise it could block the story

		if (Item.Consumable) Inventory->Mod(N, -1);
		Items.RemoveAtSwap(i);
		break; // only use one at a time.
	}

	if (UNLIKELY(Items.IsEmpty())) // unlikely since only happens once. not checking used in case someone decided to modify the Items array.
		Unlock();
	else if (Used & UseStateInc) // on the else since Trigger always increases state
		SetState(State+1);
	return Super::TryTrigger_Implementation(); // will trigger locked if nothing is used
#endif
}
