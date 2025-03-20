// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "InventoryItemUI.h"

void UInventoryItemUI::SetItem_Implementation(const FName& IName, const FItem& Item) {
	Name = IName;
}

void UInventoryItemUI::Use_Implementation() {
	if (UNLIKELY(!AUse)) return;
	PlayAnimation(AUse, 0, 1, EUMGSequencePlayMode::Forward, AnimUseSpeed);
}

void UInventoryItemUI::SetSelected_Implementation(const bool Selected) {
	IsSelected = Selected;
}
