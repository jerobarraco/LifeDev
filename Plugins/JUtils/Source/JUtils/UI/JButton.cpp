// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJButton::DoClick_Implementation() {
	OnClick.Broadcast(Id);
}

void UJButton::SetUp(const FText& NewText, const int32 NewId) {
	if (LIKELY(Text)) Text->SetText(NewText);
	Id = NewId;
}

void UJButton::SetStyle(USlateWidgetStyleAsset* const BtnStyle, USlateWidgetStyleAsset* const TextStyle) {
	if (LIKELY(BtnStyle)) {
		const FButtonStyle* const BtnStyleF = BtnStyle->GetStyle<FButtonStyle>();
		if (LIKELY(!!Btn & !!BtnStyleF)) Btn->SetStyle(*BtnStyleF);
	}
	if (LIKELY(TextStyle)) {
		const FTextBlockStyle* const TextStyleF = TextStyle->GetStyle<FTextBlockStyle>();
		if (LIKELY(!!Text & !!TextStyleF)) {
			Text->SetColorAndOpacity(TextStyleF->ColorAndOpacity);
			Text->SetFont(TextStyleF->Font);
			Text->SetShadowColorAndOpacity(TextStyleF->ShadowColorAndOpacity);
			Text->SetStrikeBrush(TextStyleF->StrikeBrush);
			Text->SetShadowOffset(TextStyleF->ShadowOffset);
			Text->SetTextOverflowPolicy(TextStyleF->OverflowPolicy);
			// have to set it manually as it does not have a setstyle :'/
		}
	}
}

void UJButton::NativeConstruct() {
	Super::NativeConstruct();
	ResetStyle();
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
