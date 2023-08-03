// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "DialogUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void UDialogUI::Hide_Implementation() {}

void UDialogUI::Show_Implementation(const FDialog& Diag) {
	PreShow();
}

void UDialogUI::Skip_Implementation() {}

void UDialogUI::PostHide() {
	// i've disabled changing the input mode and showing the mouse cursor because
	// a) it creates an issue where i need to click again before clicks are registered
	// b) it breaks immersion.
	// c) is not needed
	// d) not cross-platform
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// these are not needed since we are using the input actions
	// Controller->bShowMouseCursor = false;
	// UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	SetVisibility(ESlateVisibility::Hidden);
}

void UDialogUI::PreShow() {
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Controller->bShowMouseCursor = true;
	// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	SetVisibility(ESlateVisibility::Visible);
}
