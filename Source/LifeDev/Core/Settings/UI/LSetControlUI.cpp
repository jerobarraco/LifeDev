// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "UserSettings/EnhancedInputUserSettings.h"

#include "JButton.h"
#include "JUtils/Misc/JUtilsSys.h"

void ULSetControlUI::Apply_Implementation() {
	Super::Apply_Implementation();
	UEnhancedInputUserSettings* Settings = UJUtilsSys::GetEInputSettings(this);
	if (LIKELY(Settings)) Settings->AsyncSaveSettings();
}

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	if (LIKELY(BDefaults))
		BDefaults->OnClick.AddUniqueDynamic(this, &ULSetControlUI::SetDefaults);
}

void ULSetControlUI::NativeDestruct() {
	if (LIKELY(BDefaults))
		BDefaults->OnClick.RemoveAll(this);
	Super::NativeDestruct();
}

void ULSetControlUI::SetDefaults(const int32 Id) {
	UJUtilsSys::ResetEInputMapsAll(this);
	// todo this requires to enable the input user settings on the project setttings
	// but when i do no input works, potentially because they need to be registered with the settings???
}
