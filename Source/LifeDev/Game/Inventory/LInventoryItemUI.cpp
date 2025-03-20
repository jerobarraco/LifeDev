 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryItemUI.h"

#include "Inventory/InventoryTypes.h"

#include "LifeDev/Core/Consts/ConstColors.h"

void UInventoryItemUI::SetItem_Implementation(const FName& Name, const FItem& Item) {}

FText UInventoryItemUI::GetItemCountText(const FItem& Item) {
	static FText TNone;
	static FText TCooling = FText::FromString(TEXT("..."));
	static FText TLocked = FText::FromString(TEXT("X"));
	static FText TUnlimited = FText::FromString(TEXT("∞"));  // the font has this one, but nothing more. ⛔ 🚫 ⏱ ⏲🔒

	if (Item.Consumable)
		return FText::FromString(FString::FromInt(Item.Count)); // can't return a ref due to this

	if (!Item.SelfUsable || !Item.Usable)
		return TNone;

	// TEXT is necessary for utf
	if (Item.ActiveCoolDown > 0)
		return TCooling;

	if (Item.Locked)
		return TLocked;

	return TUnlimited;
}

FSlateColor UInventoryItemUI::GetItemColor(const FItem& Item) {
	constexpr uint8 Shade = 6;
	if (Item.Locked || Item.ActiveCoolDown>0)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Red][Shade]);
	if (Item.SelfUsable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Green][Shade]);
	if (Item.Usable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::Cyan][Shade]);
	if (Item.Consumable)
		return FColor(LDConsts::Colors::Palette[LDConsts::Colors::BlueLight][Shade]);
	return  FColor(LDConsts::Colors::Palette[LDConsts::Colors::Orange][Shade]);
}
