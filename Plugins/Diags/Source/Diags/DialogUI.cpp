// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "DialogUI.h"

void UDialogUI::ShowDlg_Implementation(const FDialog& Diag) {
	Dlgs.Add(Diag); // needs to be before show or it will fail
	Show();
	// bp has logic here. TODO port
}

void UDialogUI::Show_Implementation() {
	Super::Show_Implementation();

	// TODO port
	// APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// Controller->bShowMouseCursor = true;
	// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, this);
	// SetVisibility(ESlateVisibility::Visible);
}

void UDialogUI::ShowCurDlg_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("DialogUI::%hs DlgI=%i"), __func__, CurDlgI);
	// TODO port
}

UDialogUI::UDialogUI():Super() {
	ShowCursor = false;
	AutoUnbind = false; // critical, since the manager calls hide at the start
}

void UDialogUI::Hide_Implementation() {
	CurDlgI = 0;
	Dlgs.Empty(); // important in case they call show again while it's animating.
}

void UDialogUI::Skip_Implementation() {}
void UDialogUI::Back_Implementation() {
	SkipBy(-1);
}

void UDialogUI::SkipBy_Implementation(const int32 Diff) {
	const int32 NewDlgI = CurDlgI+Diff;
	if (NewDlgI<0) {
		UE_LOG(LogTemp, Log, TEXT("DialogUI::%hs Attempt to go to a dlg <0"), __func__);
		return;
	}
	// notice the <=. important to be able to show new dialogs after skip
	// i'm not super happy about this code, but it's ok.
	// we can't expect that the done will add the dialog synchronously. that's bad design.
	if (NewDlgI<=Dlgs.Num()) CurDlgI = NewDlgI;
	
	if (NewDlgI>=Dlgs.Num()) {
		UE_LOG(LogTemp, Log, TEXT("DialogUI::%hs Reached end of Dlgs."), __func__);
		Done();
		return;
	}
	
	ShowCurDlg();
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