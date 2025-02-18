 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryItemUI.h"

#include "Inventory/InventoryTypes.h"

#include "LifeDev/Core/Consts/ConstColors.h"

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
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Red][4]);
	if (Item.SelfUsable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Green][4]);
	if (Item.Usable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::BlueLight][4]);
	if (Item.Consumable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Cyan][4]);
	return  FColor(LDConsts::Colors::Palette[LDConsts::Colors::Orange][4]);
	// TODO maybe add some field for the colors so that it can be changed in bps
}
