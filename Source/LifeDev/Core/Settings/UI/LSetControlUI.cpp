// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "JButton.h"
#include "LInputSelector.h"
#include "JUtils/Misc/JUtilsSys.h"
#include "EnhancedInput/Public/UserSettings/EnhancedInputUserSettings.h"

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	if (LIKELY(BtnDefaults))
		BtnDefaults->OnClick.AddUniqueDynamic(this, &ULSetControlUI::SetDefaults);
}

void ULSetControlUI::NativeDestruct() {
	// todo the rest of the things?
	if (LIKELY(BtnDefaults))
		BtnDefaults->OnClick.RemoveAll(this);
	Super::NativeDestruct();
}

void ULSetControlUI::SetDefaults(const int32 Id) {
	// https://forums.unrealengine.com/t/get-enhanced-input-local-player-subsystem-in-c/1732524/2?u=nande
	// todo move to jutilssys

	ULocalPlayer* Player = GetWorld()->GetFirstLocalPlayerFromController();
	UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player);
	UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings();
	UEnhancedPlayerMappableKeyProfile* Profile = Settings->GetActiveKeyProfile();
	Profile->ResetToDefault();
}
