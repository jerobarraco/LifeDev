// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

#include "LifeDev/Core/Consts/ConstColors.h"

ULInputSelector::ULInputSelector():Super() {
	// ATM the enhanced input will make the key unusable when i rebind it
	// Super::SetIsEnabled(false);

	FTextBlockStyle S = GetTextStyle();
	// Setting the style here is a bit of a headache. so i'm just going to create a bp widget for this for the time being
	S.Font.Size = 18;
	S.SetTypefaceFontName("Roboto");
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

void ULInputSelector::KeySelected(const FInputChord Key) { // can't be ref due to how the deleagate is set
	OnKeySelectedPlus.Broadcast(this, Key);
}
