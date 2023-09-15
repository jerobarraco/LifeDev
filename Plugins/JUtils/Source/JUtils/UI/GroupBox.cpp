// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "GroupBox.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

UGroupBox::UGroupBox(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) {
}

void UGroupBox::SetSelected_Implementation(int32 NewSelected, bool Broadcast) {
	const int32 N = CheckBoxes.Num();
	// if past upper bound, then unselect
	for (uint8 i = 0; i<N; ++i) {
		CheckBoxes[i]->SetIsChecked(i==NewSelected);
	}

	Selected = NewSelected;
	if (Broadcast) {
		OnChange.Broadcast(this, NewSelected);
	}
}

void UGroupBox::SetLabel_Implementation(const FText& Text) {
	// https://benui.ca/unreal/ui-bindwidget/
	if (!Label_T) return;
	Label_T->SetText(Text);
}

void UGroupBox::NativeConstruct() {
	Super::NativeConstruct();
	for (UCheckBox* C: CheckBoxes) {
		if (!C) continue;
		C->OnCheckStateChanged.AddUniqueDynamic(this, &UGroupBox::ResetSelected);
	}
}

void UGroupBox::NativeDestruct() {
	for (UCheckBox* C: CheckBoxes) {
		if (!C) continue;
		C->OnCheckStateChanged.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UGroupBox::ResetSelected(bool bIsChecked) {
	// this i dislike but until i remember how to bind a lambda to a signal...
	Selected = -1;
	const int32 Num = CheckBoxes.Num();
	for (uint8 i = 0; i<Num; ++i){
		UCheckBox* const C = CheckBoxes[i];
		if (!C) continue;
		if (!C->IsChecked()) continue;
		if (Selected <0) {
			Selected = i;
		} else {
			C->SetIsChecked(false); // another one is set!
		}
	}

	OnChange.Broadcast(this, Selected);
}
