// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "JButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJButton::DoClick() {
	OnClick.Broadcast(Id);
}

void UJButton::SetUp(const FText& NewText, const int32 NewId) {
	if (LIKELY(Text)) Text->SetText(NewText);
	Id = NewId;
}

void UJButton::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(Btn)) Btn->OnClicked.AddUniqueDynamic(this, &UJButton::DoClick);
}

void UJButton::NativeDestruct() {
	if (LIKELY(Btn)) Btn->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}
