// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

ULInputSelector::ULInputSelector():Super() {
	// ATM the enhanced input will make the key unusable when i rebind it
	Super::SetIsEnabled(false);

	FTextBlockStyle S = GetTextStyle();
	// TODO set proper colors
	S.Font.Size = 20;
	SetTextStyle(S);

	FButtonStyle BS = GetButtonStyle();
	BS.Hovered.TintColor = FSlateColor(FColor::Red); 
	SetButtonStyle(BS);

	SetNoKeySpecifiedText(FText::FromString(TEXT("[None]")));
	SetAllowModifierKeys(false);

	SetToolTipText(FText::FromString(TEXT("Sorry. Rebinding is not available at the moment.")));
}
