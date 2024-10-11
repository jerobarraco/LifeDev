// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "DialogUI.h"

void UDialogUI::ShowDlg_Implementation(const FDialog& Diag) {
	Show();
}

void UDialogUI::Show_Implementation() {
	Super::Show_Implementation();
	
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Controller->bShowMouseCursor = true;
	// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	// SetVisibility(ESlateVisibility::Visible);
}

UDialogUI::UDialogUI():Super() {
	ShowCursor = false;
	AutoUnbind = false;
}

void UDialogUI::Hide_Implementation() {}

void UDialogUI::Skip_Implementation() {}
void UDialogUI::Back_Implementation() {
	SkipBy(-1);
}

void UDialogUI::SkipBy_Implementation(int Diff) {
	// TODO port here
}

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
	//SetVisibility(ESlateVisibility::Hidden);
	Super::Hide_Implementation();
}