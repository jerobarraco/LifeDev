// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "UserSettings/EnhancedInputUserSettings.h"

#include "JButton.h"
#include "LInputSelector.h"
#include "JUtils/Misc/JUtilsSys.h"

void ULSetControlUI::Apply_Implementation() {
	Super::Apply_Implementation();
	UEnhancedInputUserSettings* const Settings = UJUtilsSys::GetEInputSettings(this);
	if (LIKELY(Settings)) Settings->AsyncSaveSettings();
	
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->Apply();
	}
}

void ULSetControlUI::Load_Implementation() {
	Super::Load_Implementation();
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->Load();
	}
}

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	if (LIKELY(BDefaults))
		BDefaults->OnClick.AddUniqueDynamic(this, &ULSetControlUI::SetDefaults);
	
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->Init();
	}
}

void ULSetControlUI::NativeDestruct() {
	if (LIKELY(BDefaults))
		BDefaults->OnClick.RemoveAll(this);
	
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->DeInit();
	}
	Super::NativeDestruct();
}

void ULSetControlUI::SetDefaults(const int32 Id) {
	UJUtilsSys::ResetEInputMapsAll(this);
	Load();
}
