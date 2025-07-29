// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJButton::DoClick_Implementation() {
	OnClick.Broadcast(Id);
}

void UJButton::SetUp(const FText& NewText, const int32 NewId) {
	if (LIKELY(Text)) Text->SetText(NewText);
	Id = NewId;
}

void UJButton::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (UNLIKELY(!Btn)) return;

	Btn->OnClicked.AddUniqueDynamic(this, &UJButton::DoClick);
}

void UJButton::NativeDestruct() {
	if (LIKELY(Btn)) Btn->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}
