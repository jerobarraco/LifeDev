// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "DialogUI.h"

void UDialogUI::Hide_Implementation() {}

void UDialogUI::Show_Implementation(const FDialog& Diag) {
	UE_LOG(LogTemp, Warning, TEXT("Dialog UI has not bounded the show function"));
}

void UDialogUI::Skip_Implementation() {}
