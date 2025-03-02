// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "OutroUI.h"

#include "JButton.h"
#include "Components/WidgetSwitcher.h"

void UOutroUI::SetFlags_Implementation(const TMap<FName, float>& Flags) {
	// TODO port here
}

void UOutroUI::SetPage_Implementation(const int32 Page) {} // bp. //todo port here

void UOutroUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	// if (LIKELY(BtnRetry)) BtnRetry->SetUp()
	if (LIKELY(BtnStats)) {
		BtnStats->SetUp(NSLOCTEXT("Outro", "BtnStats", "Stats"), 2);
		BtnStats->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	}

	if (LIKELY(BtnCredits)) {
		BtnCredits->SetUp(NSLOCTEXT("Outro", "BtnCredits", "Credits"), 1);
		BtnCredits->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	}

	if (LIKELY(BtnBack)) {
		BtnBack->SetUp(NSLOCTEXT("Outro", "BtnBack", "Back"), 0);
		BtnBack->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	}

	if (LIKELY(BtnBack2)) {
		BtnBack2->SetUp(NSLOCTEXT("Outro", "BtnBack2", "Back"), 0);
		BtnBack2->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	}
	
}

