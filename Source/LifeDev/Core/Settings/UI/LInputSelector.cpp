// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

ULInputSelector::ULInputSelector():Super() {
	// ATM the enhanced input will make the key unusable when i rebind it
	// Super::SetIsEnabled(false);

	// this is so good 
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CSB(TEXT("/Game/LifeDev/Core/UI/Btns/LButtonSmall_S"));
	StyleBtn = CSB.Object;
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CST(TEXT("/Game/LifeDev/Core/UI/Btns/LTextSmall_S"));
	StyleText = CST.Object;

	SetNoKeySpecifiedText(NSLOCTEXT("InputSelector", "NoKey", "[None]"));
	SetKeySelectionText(NSLOCTEXT("InputSelector", "Waiting", "[Waiting...]"));
	SetAllowModifierKeys(false);
	EscapeKeys.Add(EKeys::Escape);
	EscapeKeys.Add(EKeys::Gamepad_Special_Right);
	
	// SetToolTipText(FText::FromString(TEXT("Sorry. Rebinding is not available at the moment.")));
}

void ULInputSelector::Init(const FInputChord& Key) {
	SetSelectedKey(Key);
	OnKeySelected.AddUniqueDynamic(this, &ULInputSelector::KeySelected);
}

void ULInputSelector::DeInit() {
	OnKeySelected.RemoveAll(this);
}

void ULInputSelector::ResetStyle() {
	if (StyleBtn) {
		const FButtonStyle* const Style = StyleBtn->GetStyle<FButtonStyle>();
		if (LIKELY(Style)) SetButtonStyle(*Style);
	}

	if (StyleText) {
		const FTextBlockStyle* const Style = StyleText->GetStyle<FTextBlockStyle>();
		if (LIKELY(Style)) SetTextStyle(*Style);
	}
}

void ULInputSelector::SetDefault() {
	// TODO
}

void ULInputSelector::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}

void ULInputSelector::KeySelected(const FInputChord Key) { // can't be ref due to how the deleagate is set
	OnKeySelectedPlus.Broadcast(this, Key);
}
