 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryUI.h"

#include "Inventory/InventoryItemUI.h"

void ULInventoryUI::FadeUsed(const bool Fwd) { // todo move to uinventoryui
	if (UNLIKELY(!AUsed)) return;

	PlayAnimation(AUsed, 0, 1,
		Fwd ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, 1);
}

void ULInventoryUI::Show_Implementation() {
	PreShow();
	Super::Show_Implementation();
}

void ULInventoryUI::Hide_Implementation() {
	Super::Hide_Implementation();
	PostHide();
}

void ULInventoryUI::SetItemMod_Implementation(const FName& Name, const int32 Diff, const FItem& Item) {
	Super::SetItemMod_Implementation(Name, Diff, Item);

	if (UNLIKELY(Name.IsNone())) return;

	if (Item.Count<=0) {
		Items.Remove(Name);
		// todo fade, add timer. then remove.
		return;
	}

	TObjectPtr<UInventoryItemUI>* const pIt = Items.Find(Name);
	UInventoryItemUI* const It = pIt ? pIt->Get() : nullptr;
	if (!pIt) {
		// todo create here
	}

	if (UNLIKELY(!It)) return; // safeguard
	It->SetItem(Name, Item);
}
