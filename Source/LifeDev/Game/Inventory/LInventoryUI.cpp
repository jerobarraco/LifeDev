 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryUI.h"
void ULInventoryUI::FadeUsed(const bool Fwd) { // todo move to uinventoryui
	if (UNLIKELY(!AUsed)) return;

	PlayAnimation(AUsed, 0, 1,
		Fwd ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, 1);
}

void ULInventoryUI::SetItemMod_Implementation(const FName& Name, const int32 Diff, const FItem& Item) {
	Super::SetItemMod_Implementation(Name, Diff, Item);

	if (Item.Count<=0) {
		Items.Remove(Name);
		// todo fade
		return;
	}
	
}
