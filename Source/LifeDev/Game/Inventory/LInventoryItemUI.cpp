 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryItemUI.h"

#include "Inventory/InventoryTypes.h"

void UInventoryItemUI::SetItem_Implementation(const FName& Name, const FItem& Item) {}
FText UInventoryItemUI::GetItemCountText(const FItem& Item) {
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

FSlateColor UInventoryItemUI::GetItemColor(const FItem& Item) {
	if (Item.Locked || Item.ActiveCoolDown>0)
		return FColor::Red; // TODO
	if (Item.SelfUsable)
		return FColor::Green; // TODO
	if (Item.Usable)
		return FColor::Cyan;
	if (Item.Consumable)
		return FColor::Yellow; // TODO
	return FColor::Black; // TODO maybe add some field for the colors so that it can be changed in bps
}
