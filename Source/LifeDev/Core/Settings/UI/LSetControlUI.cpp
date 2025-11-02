// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "JButton.h"
#include "JUtils/Misc/JUtilsSys.h"

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	if (LIKELY(BDefaults))
		BDefaults->OnClick.AddUniqueDynamic(this, &ULSetControlUI::SetDefaults);
}

void ULSetControlUI::NativeDestruct() {
	if (LIKELY(BDefaults))
		BDefaults->OnClick.RemoveAll(this);
	Super::NativeDestruct();
}

void ULSetControlUI::SetDefaults(const int32 Id) {
	UJUtilsSys::ResetEInputMapsAll(this);
}
