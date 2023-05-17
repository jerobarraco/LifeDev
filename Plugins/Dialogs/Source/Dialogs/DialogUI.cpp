// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "DialogUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UDialogUI::Hide_Implementation() {}

void UDialogUI::Show_Implementation(const FDialog& Diag) {
	UE_LOG(LogTemp, Warning, TEXT("Dialog UI has not bounded the show function"));
}

void UDialogUI::Skip_Implementation() {
	if (IsReady) OnDone.Broadcast();
}

void UDialogUI::FinishHide() {
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	SetVisibility(ESlateVisibility::Hidden);
}

void UDialogUI::PreShow() {
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	SetVisibility(ESlateVisibility::Visible);
}
