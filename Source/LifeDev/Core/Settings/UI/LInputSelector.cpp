// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

ULInputSelector::ULInputSelector():Super() {
	// ATM the enhanced input will make the key unusable when i rebind it
	// Super::SetIsEnabled(false);
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CS(TEXT("/Game/LifeDev/Core/UI/Btns/LButton_S"));
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CST(TEXT("/Game/LifeDev/Core/UI/Btns/LButtonText_S"));
	USlateWidgetStyleAsset* const StyleAssB = CS.Object;
	if (StyleAssB) {
		const FButtonStyle* const Style = CS.Object->GetStyle<FButtonStyle>();
		SetButtonStyle(*Style);
	}
	USlateWidgetStyleAsset* const StyleAssT = CST.Object;
	if (StyleAssT) {
		const FTextBlockStyle* const Style = CST.Object->GetStyle<FTextBlockStyle>();
		SetTextStyle(*Style);
	}

	/*
	FTextBlockStyle S = GetTextStyle();
	// Setting the style here is a bit of a headache.
	// i considered using a widget, but i can't subclass it, i have to wrap it.
	// and that makes it difficult to communicate with this code. so no. 
	// S.Font.Size = 18;
	// S.SetTypefaceFontName("Roboto");
	S.ColorAndOpacity = FSlateColor(FColor(LDConsts::Colors::Palette[LDConsts::Colors::Grey][14]));
	FSlateFontInfo FI;
	FI.OutlineSettings.OutlineSize = 1;
	FI.OutlineSettings.OutlineColor = FColor(LDConsts::Colors::Palette[LDConsts::Colors::Grey][1]);
	FI.TypefaceFontName = "Roboto";
	FI.Size = 18;
	// S.SetFont(MoveTemp(FI));
	// S.ColorAndOpacity = FSlateColor(FColor(LDConsts::Colors::Palette[LDConsts::Colors::Blue][3]));
	
	SetTextStyle(S);
	
	// FButtonStyle BS = GetButtonStyle();
	// BS.Normal.TintColor = FSlateColor(FColor(LDConsts::Colors::Palette[LDConsts::Colors::Grey][5]));
	// BS.Hovered.TintColor = FSlateColor(FColor(LDConsts::Colors::Palette[LDConsts::Colors::BlueLight][5]));
	// BS.Pressed.TintColor = FSlateColor(FColor(LDConsts::Colors::Palette[LDConsts::Colors::BlueLight][8]));
	
	*/
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
