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
		OnChange.Broadcast(ID, NewSelected);
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
		UCBChangeWrapper* const Wrapper = NewObject<UCBChangeWrapper>();
		Wrapper->CB = C;
		Wrapper->OnChange.AddUniqueDynamic(this, &UGroupBox::ResetSelected);
		C->OnCheckStateChanged.AddUniqueDynamic(Wrapper, &UCBChangeWrapper::Dispatch);
	}
}

void UGroupBox::NativeDestruct() {
	for (UCheckBox* C: CheckBoxes) {
		if (!C) continue;
		// todo this is fake is not actually removing the wrapper. fix
		C->OnCheckStateChanged.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UGroupBox::ResetSelected(UCheckBox* CB, bool IsChecked) {
	if (!IsChecked) {
		CB->SetIsChecked(true); // don't allow to manually deselect
	}

	Selected = -1;
	const int32 Num = CheckBoxes.Num();
	for (uint8 i = 0; i<Num; ++i){
		UCheckBox* const C = CheckBoxes[i];
		if (!C) continue;
		
		if (!C->IsChecked()) continue;
		
		if (C == CB) {
			Selected = i;
		} else {
			// this is not re-triggering the delegate, otherwise it will be tragic.
			C->SetIsChecked(false); // clear the others
		}
	}

	OnChange.Broadcast(ID, Selected);
}
