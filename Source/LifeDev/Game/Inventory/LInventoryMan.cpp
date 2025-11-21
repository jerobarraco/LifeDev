// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryMan.h"

#include "Diags/Diags.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LInventoryUI.h"

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
	FeatUp(EFeat::G_SHOW_INV, Settings->GetFeat(EFeat::G_SHOW_INV));

	// disable the input while dialogs are showing.
	// the input priority will only eat the keys that i use, not the rest.
	// and even that i'm not sure that it works well.
	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnShow.AddUniqueDynamic(this, &ALInventoryMan::DiagShow);
		Diags->OnDone.AddUniqueDynamic(this, &ALInventoryMan::DiagDone);
	}
}

void ALInventoryMan::DeInit_Implementation() {
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings))
		Settings->OnFeatUpdateGameplay.RemoveAll(this);

	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnShow.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
	}
	Super::DeInit_Implementation();
}

void ALInventoryMan::FeatUp(const EFeat Feat, const bool Enabled) {
	if (Feat != EFeat::G_SHOW_INV) return;
	IsShowEnabled = Enabled; // flag is to hide.

	// this could break it if the feat changes while a dialog is showing.
	// but that can't happen, the settings menu can't be accessed during a dialog.
	if (IsShowEnabled) Show();
	else Hide();
}
