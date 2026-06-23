// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "Engine/UserInterfaceSettings.h"
#include "GameFramework/PlayerController.h"

void UJUtilsUI::ShowUI(const UObject* const O, const bool Show,  UWidget* const Focus) {
	if (UNLIKELY(!IsValid(O))) return;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	APlayerController* const Controller = World->GetFirstPlayerController();
	if (UNLIKELY(!IsValid(Controller))) return;

	if (Show) {
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
			Controller, Focus, EMouseLockMode::DoNotLock, true);
	} else UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);

	Controller->SetShowMouseCursor(Show);
}

void UJUtilsUI::SetUIScale(const float UIScale) {
	// inspired on https://benui.ca/unreal/ui-scale/
	UUserInterfaceSettings* const UISettings =
		GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());

	if (UNLIKELY(!UISettings)) return;
	UISettings->ApplicationScale = UIScale;
}

float UJUtilsUI::GetUIScale() {
	const UUserInterfaceSettings* const UISettings =
		GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());

	if (UNLIKELY(!UISettings)) return 1;
	return UISettings->ApplicationScale;
}

bool UJUtilsUI::GetStyleText(USlateWidgetStyleAsset* const Asset, FTextBlockStyle& Ret) {
	if (UNLIKELY(!Asset)) return false;
	Ret = *Asset->GetStyle<FTextBlockStyle>(); // copy
	return true;
}

bool UJUtilsUI::GetStyleButton(USlateWidgetStyleAsset* const Asset, FButtonStyle& Ret) {
	if (UNLIKELY(!Asset)) return false;
	Ret = *Asset->GetStyle<FButtonStyle>(); // copy
	return true;
}
