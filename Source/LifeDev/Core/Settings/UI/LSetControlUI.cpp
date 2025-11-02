// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "JButton.h"
#include "LInputSelector.h"

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	if (LIKELY(BtnDefaults))
		BtnDefaults->OnClick.AddUniqueDynamic(this, &ULSetControlUI::SetDefaults);
}

void ULSetControlUI::NativeDestruct() {
	// todo the rest of the things?
	if (LIKELY(BtnDefaults))
		BtnDefaults->OnClick.RemoveAll(this);
	Super::NativeDestruct();
}

void ULSetControlUI::SetDefaults(const int32 Id) {
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		S->SetDefault();
	}
}
