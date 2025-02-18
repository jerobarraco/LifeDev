 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryItemUI.h"

#include "Inventory/InventoryTypes.h"

void UInventoryItemUI::SetItem_Implementation(const FName& Name, const FItem& Item) {}
FText UInventoryItemUI::ItemCountText(const FItem& Item) {
	if (Item.Consumable)
		return FText::FromString(FString::FromInt(Item.Count));

	if (!Item.SelfUsable || !Item.Usable)
		return FText();

	if (Item.ActiveCoolDown > 0)
		return FText::FromString("...");

	if (Item.Locked)
		return FText::FromString("X");

	return FText::FromString(TEXT("∞"));
}
