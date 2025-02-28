// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"

#include "Components/ComboBoxString.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "JUtils/UI/GroupBox.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"

ULSettingsUI::ULSettingsUI():Super() {
	// note: the intro level intro ui will set `ShowCursor` to false. to avoid having the mouse hidden.
	ShowCursor = true;
	AutoUnbind = false; // you only bind once.
}

void ULSettingsUI::Show_Implementation() {
	// SetVisibility(ESlateVisibility::Visible);
	Super::Show_Implementation();

	const ALMusicMan* const Man = ALMusicMan::Instance(this);
	if (LIKELY(Man)) Man->FadeFX(true);

	Load();
}

void ULSettingsUI::Hide_Implementation() {
	const ALMusicMan* const Man = ALMusicMan::Instance(this);
	if (LIKELY(Man)) Man->FadeFX(false);
	
	Super::Hide_Implementation();
	// SetVisibility(ESlateVisibility::Collapsed);
}

void ULSettingsUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(CBScale)) {
		CBScale->ClearOptions();
		CBScale->AddOption("UI Scale");
		CBScale->AddOption("x .25 Scale");
		CBScale->AddOption("x .5 Scale");
		CBScale->AddOption("x .75 Scale");
		CBScale->AddOption("x 1 Scale");
		CBScale->AddOption("x 1.25 Scale");
		CBScale->AddOption("x 1.5 Scale");
		CBScale->AddOption("x 1.75 Scale");
		CBScale->AddOption("x 2 Scale");
		CBScale->AddOption("x 3 Scale");
		CBScale->AddOption("x 4 Scale");
		CBScale->OnSelectionChanged.AddUniqueDynamic(this, &ULSettingsUI::ScaleUpd);
	}
}

void ULSettingsUI::NativeDestruct() {
	if (CBScale) CBScale->OnSelectionChanged.RemoveAll(this);

	Super::NativeDestruct();
}

void ULSettingsUI::ScaleUpd(const FString SelectedItem, const ESelectInfo::Type SelectionType) {
	constexpr float Scales[] = {1, .25, .5, .75, 1, 1.25, 1.5, 1.75, 2, 3, 4};
	constexpr uint8 Len = UJUtilsMisc::ArraySize(Scales);
	if (UNLIKELY(!CBScale)) return;

	const int32 I = CBScale->GetSelectedIndex();
	if (UNLIKELY(I<1 || I>=Len)) return; // actually ignore 0
	UJUtilsMisc::SetUIScale(Scales[I]);
}
