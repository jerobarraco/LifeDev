// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "JButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJButton::DoClick() {
	OnClick.Broadcast(Id);
}

void UJButton::SetUp(const FText& NewText, const int32 NewId) {
	if (Text) Text->SetText(NewText);
	Id = NewId;
}

void UJButton::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (Btn) Btn->OnClicked.AddUniqueDynamic(this, &UJButton::DoClick);
}

void UJButton::NativeDestruct() {
	if (Btn) Btn->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}
