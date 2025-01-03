// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"

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
	if (Man) Man->FadeFX(false);
	
	Super::Hide_Implementation();
	
	// SetVisibility(ESlateVisibility::Collapsed);
}

void ULSettingsUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
}

void ULSettingsUI::NativeDestruct() {
	Super::NativeDestruct();
}
