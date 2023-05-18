// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "DialogUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

// TODO fix issue where i need to click again after hiding the ui to regain control of player

void UDialogUI::Hide_Implementation() {}

void UDialogUI::Show_Implementation(const FDialog& Diag) {
	UE_LOG(LogTemp, Warning, TEXT("Dialog UI has not bounded the show function"));
}

void UDialogUI::Skip_Implementation() {
	if (IsReady) OnDone.Broadcast();
}

void UDialogUI::FinishHide() {
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// these are not needed since we are using the input actions
	// Controller->bShowMouseCursor = false;
	// UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	SetVisibility(ESlateVisibility::Hidden);
}

void UDialogUI::PreShow() {
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Controller->bShowMouseCursor = true;
	// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	SetVisibility(ESlateVisibility::Visible);
}
