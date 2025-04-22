// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryItemUI.h"

#include "Inventory/InventoryTypes.h"

#include "LifeDev/Core/Consts/ConstColors.h"

ULInventoryItemUI::ULInventoryItemUI() {
	AnimUseSpeed = 2;
}

FText ULInventoryItemUI::GetItemCountText(const FItem& Item) {
	static const FText TNone;
	static const FText TCooling = FText::FromString(TEXT("..."));
	static const FText TLocked = FText::FromString(TEXT("X"));
	static const FText TUnlimited = FText::FromString(TEXT("∞"));  // the font has this one, but nothing more. ⛔ 🚫 ⏱ ⏲🔒

	if (Item.Consumable)
		return FText::FromString(FString::FromInt(Item.Count)); // can't return a ref due to this

	if (!Item.SelfUsable | !Item.Usable)
		return TNone;

	// TEXT is necessary for utf
	if (Item.ActiveCoolDown > 0)
		return TCooling;

	if (Item.Locked)
		return TLocked;

	return TUnlimited;
}

const FSlateColor& ULInventoryItemUI::GetItemColor(const FItem& Item) {
	constexpr uint8 Shade = 6;
	static const FSlateColor CLocked = FColor(LDConsts::Colors::Palette[LDConsts::Colors::Red][Shade]);
	static const FSlateColor CUseSelf = FColor(LDConsts::Colors::Palette[LDConsts::Colors::Green][Shade]);
	static const FSlateColor CUse = FColor(LDConsts::Colors::Palette[LDConsts::Colors::Cyan][Shade]);
	static const FSlateColor CConsume = FColor(LDConsts::Colors::Palette[LDConsts::Colors::BlueLight][Shade]);
	static const FSlateColor CDefault = FColor(LDConsts::Colors::Palette[LDConsts::Colors::Orange][Shade]);
	
	if (Item.Locked || Item.ActiveCoolDown>0)
		return CLocked;
	if (Item.SelfUsable)
		return CUseSelf;
	if (Item.Usable)
		return CUse;
	if (Item.Consumable)
		return CConsume;
	return CDefault;
}
