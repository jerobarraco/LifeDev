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

	// TEXT is necessary for utf
	if (Item.ActiveCoolDown > 0)
		return FText::FromString(TEXT("..."));

	if (Item.Locked)
		return FText::FromString(TEXT("X"));

	return FText::FromString(TEXT("∞")); // the font has this one, but nothing more. ⛔ 🚫 ⏱ ⏲🔒
}

FSlateColor UInventoryItemUI::GetItemColor(const FItem& Item) {
	constexpr uint8 Shade = 6;
	if (Item.Locked || Item.ActiveCoolDown>0)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Red][Shade]);
	if (Item.SelfUsable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Green][Shade]);
	if (Item.Usable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::BlueLight][Shade]);
	if (Item.Consumable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Cyan][Shade]);
	return  FColor(LDConsts::Colors::Palette[LDConsts::Colors::Orange][Shade]);
}
