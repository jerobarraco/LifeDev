// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "OutroUI.h"

#include "Components/WidgetSwitcher.h"

#include "JButton.h"

#include "LifeDev/Game/Flashback/Flashback.h"

void UOutroUI::SetFlags_Implementation(const TMap<FName, float>& Flags) {
	// TODO port here
}

void UOutroUI::SetPage_Implementation(const int32 Page) {
	if (LIKELY(Switcher)) Switcher->SetActiveWidgetIndex(Page);

	UFlashback* const Flash = UFlashback::Instance(this);
	if (UNLIKELY(!Flash)) return;

	Flash->SetVal(Page==0?1:.6, .75);
}

void UOutroUI::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (UNLIKELY(!Switcher)) return;
	
	if (LIKELY(BtnStats)) {
		BtnStats->SetUp(NSLOCTEXT("Outro", "BtnStats", "Stats"), 2);
		BtnStats->OnClick.AddUniqueDynamic(this, &UOutroUI::SetPage);
	}

	if (LIKELY(BtnCredits)) {
		BtnCredits->SetUp(NSLOCTEXT("Outro", "BtnCredits", "Credits"), 1);
		BtnCredits->OnClick.AddUniqueDynamic(this, &UOutroUI::SetPage);
	}

	if (LIKELY(BtnBack)) {
		BtnBack->SetUp(NSLOCTEXT("Outro", "BtnBack", "Back"), 0);
		BtnBack->OnClick.AddUniqueDynamic(this, &UOutroUI::SetPage);
	}

	if (LIKELY(BtnBack2)) {
		BtnBack2->SetUp(NSLOCTEXT("Outro", "BtnBack2", "Back"), 0);
		BtnBack2->OnClick.AddUniqueDynamic(this, &UOutroUI::SetPage);
	}

	if (LIKELY(BtnRetry)) {
		BtnRetry->SetUp(NSLOCTEXT("Outro", "BtnRetry", "Retry"), 0);
		BtnRetry->OnClick.AddUniqueDynamic(this, &UBaseUI::Done);
	}

	if (LIKELY(BtnQuit)) {
		BtnQuit->SetUp(NSLOCTEXT("Outro", "BtnQuit", "Quit"), 1);
		BtnQuit->OnClick.AddUniqueDynamic(this, &UBaseUI::Done);
	}
}

