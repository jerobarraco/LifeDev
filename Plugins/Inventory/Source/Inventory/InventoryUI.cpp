// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "InventoryUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UInventoryUI::Hide_Implementation() {}

void UInventoryUI::Show_Implementation() {}

void UInventoryUI::SetSelected_Implementation(const FName& Name) {}

void UInventoryUI::PostHide() {
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// these are not needed since we are using the input actions
	Controller->bShowMouseCursor = false;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUI::PreShow() {
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Controller->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	SetVisibility(ESlateVisibility::Visible);
}
