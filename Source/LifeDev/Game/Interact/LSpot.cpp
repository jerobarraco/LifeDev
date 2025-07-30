#include "LSpot.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"

#include "LifeDev/Core/Consts/ConstDlgs.h"

ALSpot::ALSpot():Super() {
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

bool ALSpot::TryTrigger_Implementation() {
	// somewhat similar to should unlock. which is not great.
	// consume items when unlocking

	bool Used = false;
	for (int32 i = Items.Num()-1; i>=0; --i) {
		const FName& N = Items[i];
		FItem Item;
		Used = Inventory->Get(N, Item);
		if (!Used) continue; // not return, so that it can consume the rest, otherwise it could block the story

		if (Item.Consumable) Inventory->Mod(N, -1);

		// trigger the dialog here. avoid extra if below
		const FString& Base = LDConsts::Dlgs::Inter::UseItemPre + Label.ToString();
		const bool Added = Diags->AddId(FName(Base+"."+N.ToString())); // this is crashing. 
		if (!Added) Diags->AddId(FName(Base));

		Items.RemoveAtSwap(i); // removed here, otherwise N.ToString will crash since it's a ref. or i could copy it. but i rather not.
		break; // only use one at a time.
	}

	// unlikely since only happens once. not checking used in case someone decided to modify the Items array.
	if (UNLIKELY(Items.IsEmpty())) {
		Unlock();
		return Super::TryTrigger_Implementation(); // will trigger locked if nothing is used. will trigger some dialogs.
	}

	// will trigger locked if nothing is used. will trigger some dialogs.
	if (!Used) return Super::TryTrigger_Implementation();

	// depends on !Used above returning. Trigger always increases state
	if (UseStateInc) SetState(State+1);

	return true;
}
