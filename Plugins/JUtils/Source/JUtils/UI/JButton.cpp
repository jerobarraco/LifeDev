// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJButton::DoClick_Implementation() {
	OnClick.Broadcast(Id);
}

void UJButton::SetUp(const FText& NewText, const int32 NewId) {
	Id = NewId;
	Label = NewText; // important in case this function is called before widget rebuild
	if (LIKELY(Text)) Text->SetText(Label);
}

void UJButton::ResetStyle() {
	if (LIKELY(BtnStyle)) {
		const FButtonStyle* const BtnStyleF = BtnStyle->GetStyle<FButtonStyle>();
		if (LIKELY(bool(Btn) & bool(BtnStyleF))) Btn->SetStyle(*BtnStyleF);
	}
	if (LIKELY(TextStyle)) {
		const FTextBlockStyle* const TextStyleF = TextStyle->GetStyle<FTextBlockStyle>();
		if (LIKELY(bool(Text) & bool(TextStyleF))) {
			Text->SetColorAndOpacity(TextStyleF->ColorAndOpacity);
			Text->SetFont(TextStyleF->Font);
			Text->SetShadowColorAndOpacity(TextStyleF->ShadowColorAndOpacity);
			Text->SetStrikeBrush(TextStyleF->StrikeBrush);
			Text->SetShadowOffset(TextStyleF->ShadowOffset);
			Text->SetTextOverflowPolicy(TextStyleF->OverflowPolicy);
			// have to set it manually as it does not have a setstyle :'/
		}
	}

	if (LIKELY(Text)) Text->SetText(Label);
}

void UJButton::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	// NativePreConstruct i could use this. but this only exists on user widgets,
	// so it doesn't exist on all objects, e.g. ComboBoxes.
	// so i rather use WidgetRebuilt to keep consistency as i don't have a strong preference atm for both.
	ResetStyle(); // this actually happens when i need to
}

void UJButton::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (UNLIKELY(!Btn)) return;

	Btn->OnClicked.AddUniqueDynamic(this, &UJButton::DoClickInt);
}

void UJButton::NativeDestruct() {
	if (LIKELY(Btn)) Btn->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}
// void UJButton::NativeConstruct() {// only happens during gameplay and not always. it's not useful for reset style
