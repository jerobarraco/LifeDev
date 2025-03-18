// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryMan.h"

#include "LInventoryUI.h"
#include "LifeDev/Core/Settings/LSettings.h"

ALInventoryMan::ALInventoryMan():Super() {
	static ConstructorHelpers::FClassFinder<ULInventoryUI>
		CUI(TEXT("/Game/LifeDev/Game/Inventory/UI/W_InventoryUI"));
	UIClass = CUI.Succeeded() ? CUI.Class.Get() : UInventoryUI::StaticClass();
}

void ALInventoryMan::Init_Implementation() {
	Super::Init_Implementation();

	ULSettings* const Settings = ULSettings::Instance(this);
	if (UNLIKELY(!Settings)) return;
	Settings->OnFeatUpdateGameplay.AddUniqueDynamic(this, &ALInventoryMan::FeatUp);
	IsShowEnabled = ULSettings::GetFeatS(this, EFeat::G_SHOW_UI);
}

void ALInventoryMan::FeatUp(const EFeat Feat, const bool Enabled) {
	if (Feat != EFeat::G_SHOW_UI) return;
	IsShowEnabled = Enabled;
	// this could break it if the feat changes while a dialog is showing. but ... i don't care.
	if (Enabled) Show(); else Hide();
}
