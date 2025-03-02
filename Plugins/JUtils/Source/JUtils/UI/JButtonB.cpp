// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "JButtonB.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJButtonB::DoClick() {
	OnClick.Broadcast(Id);
}

void UJButtonB::SetUp(const FText& NewText, const int32 NewId) {
	if (LIKELY(Text)) Text->SetText(NewText);
	Id = NewId;
}

void UJButtonB::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(Btn)) Btn->OnClicked.AddUniqueDynamic(this, &UJButtonB::DoClick);
}

void UJButtonB::NativeDestruct() {
	if (LIKELY(Btn)) Btn->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}
