// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

ULInputSelector::ULInputSelector():Super() {
	// ATM the enhanced input will make the key unusable when i rebind it
	// Super::SetIsEnabled(false);

	// this is so good 
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CS(TEXT("/Game/LifeDev/Core/UI/Btns/LButtonSmall_S"));
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CST(TEXT("/Game/LifeDev/Core/UI/Btns/LTextSmall_S"));
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
