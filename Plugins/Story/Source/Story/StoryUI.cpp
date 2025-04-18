// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "StoryUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

void UStoryUI::NativeConstruct() {
	Super::NativeConstruct();
	ShowBGSolid(true);
	IsShowing = false;
}

void UStoryUI::ShowBGSolid(const bool Show) {
	UE_LOG(LogTemp, Log, TEXT("UStoryUI::%hs show=%i"), __func__, Show);

	if (UNLIKELY(!BGSolid)) return;
	BGSolid->SetVisibility(Show ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UStoryUI::AnimFinished() {
	OnDone.Broadcast();
}

void UStoryUI::PostHide() {
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// these are not needed since we are using the input actions
	// Controller->bShowMouseCursor = false;
	// UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	SetVisibility(ESlateVisibility::Hidden);
	IsShowing = false;
	AnimFinished();
}

void UStoryUI::PreShow() {
	IsShowing = true;
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Controller->bShowMouseCursor = true;
	// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	SetVisibility(ESlateVisibility::Visible);
}
