// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"

#include "LSettings.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "JUtils/UI/GroupBox.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"

namespace LSetUI {
	// first one should be skippable. needs to be in ascending order.
	constexpr float Scales[] = {1, .25, .5, .75, 1, 1.25, 1.5, 1.75, 2, 3, 4};
	constexpr uint8 ScalesLen = UJUtilsMisc::ArraySize(Scales);
}

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

void ULSettingsUI::Load_Implementation() {
	// doesn't work as expected and prints errors
	// if (LIKELY(Settings_Dbg)) Settings_Dbg->Load(); 
}

void ULSettingsUI::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (LIKELY(CBScale)) {
		CBScale->ClearOptions();
		CBScale->AddOption("UI Scale");
		const float ScaleCur = UJUtilsMisc::GetUIScale();
		int32 SelectedI = 0;
		for (uint8 i = 1; i<LSetUI::ScalesLen; ++i) {//skip initial
			const float S = LSetUI::Scales[i];
			CBScale->AddOption(FString::Printf(TEXT("%.2fx Scale"), S));
			// selects the last one that's smaller than the current one.
			// will pick the largest one that is not larger than the value.
			if (S<=ScaleCur) SelectedI = i;
		}
		CBScale->SetSelectedIndex(SelectedI);
		CBScale->OnSelectionChanged.AddUniqueDynamic(this, &ULSettingsUI::ScaleUpd);
	}

	const bool Dbg = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
	if (LIKELY(BtnOptDbg)) {
		BtnOptDbg->SetVisibility(Dbg ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (UNLIKELY(Dbg)) BtnOptDbg->OnClicked.AddUniqueDynamic(this, &ULSettingsUI::ShowDbg);
	}
}

void ULSettingsUI::NativeDestruct() {
	if (LIKELY(CBScale)) CBScale->OnSelectionChanged.RemoveAll(this);

	Super::NativeDestruct();
}

void ULSettingsUI::ScaleUpd(const FString SelectedItem, const ESelectInfo::Type SelectionType) {
	if (UNLIKELY(!CBScale)) return;

	const int32 I = CBScale->GetSelectedIndex();
	if (UNLIKELY(I<1 || I>=LSetUI::ScalesLen)) return; // actually ignore 0
	UJUtilsMisc::SetUIScale(LSetUI::Scales[I]);
}

void ULSettingsUI::ShowDbg() {
	if (LIKELY(SWOptions)) SWOptions->SetActiveWidgetIndex(5);
}
