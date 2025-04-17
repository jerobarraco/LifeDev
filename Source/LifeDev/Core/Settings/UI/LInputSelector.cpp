// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

#include "LifeDev/Core/Consts/ConstColors.h"

ULInputSelector::ULInputSelector():Super() {
	// ATM the enhanced input will make the key unusable when i rebind it
	// Super::SetIsEnabled(false);

	FTextBlockStyle S = GetTextStyle();
	// TODO set proper colors
	S.Font.Size = 20;
	S.ColorAndOpacity = FSlateColor(FColor(LDConsts::Colors::Palette[LDConsts::Colors::Blue][3]));
	SetTextStyle(S);
	
	FButtonStyle BS = GetButtonStyle();
	BS.Hovered.TintColor = FSlateColor(FColor(LDConsts::Colors::Palette[LDConsts::Colors::Brown][3]));
	SetButtonStyle(BS);

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

void ULInputSelector::KeySelected(FInputChord Key) {
	OnKeySelectedPlus.Broadcast(this, Key);
}
