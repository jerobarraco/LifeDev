// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "StoryUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void UStoryUI::FadeOut_Implementation() {}

void UStoryUI::FadeIn_Implementation(const FText& Title, const FText& Text) {}

void UStoryUI::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
	IsShowing = false;
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
