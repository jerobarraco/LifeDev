// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LQualitySwitchUI.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

ULQualitySwitchUI::ULQualitySwitchUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) {
}

void ULQualitySwitchUI::SetSelected_Implementation(int32 NewSelected, bool Broadcast) {
	const int32 N = CheckBoxes.Num();
	// if past upper bound, then unselect
	for (uint8 i = 0; i<N; ++i) {
		CheckBoxes[i]->SetIsChecked(i==NewSelected);
	}

	if (Broadcast) {
		OnChange.Broadcast(NewSelected);
	}
	// Selected = NewSelected;
}

void ULQualitySwitchUI::SetLabel_Implementation(const FText& Text) {
	// https://benui.ca/unreal/ui-bindwidget/
	if (!Label_T) return;
	Label_T->SetText(Text);
}
