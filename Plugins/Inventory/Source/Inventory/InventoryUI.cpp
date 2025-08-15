// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "InventoryUI.h"

#include "InventoryItemUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryUI::Hide_Implementation() {}

void UInventoryUI::Show_Implementation() {}

void UInventoryUI::SetSelected_Implementation(const FName& Name) {}

void UInventoryUI::SetItemMod_Implementation(const FName& Name, int32 Diff, const FItem& Item) {}

void UInventoryUI::SetItemCold_Implementation(const FName& Name) {}

void UInventoryUI::SetItemUsed_Implementation(const FName& Name) {}

void UInventoryUI::PostHide() {
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// these are not needed since we are using the input actions
	// Controller->bShowMouseCursor = false;
	// UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUI::PreShow() {
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Controller->bShowMouseCursor = true;
	// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	SetVisibility(ESlateVisibility::Visible);
}
