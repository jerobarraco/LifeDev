// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "GroupBox.h"

#include "DelegateWrappers.h"
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

void UGroupBox::NativeOnInitialized() {
	Super::NativeOnInitialized();
	for (const TObjectPtr<UCheckBox>& C: CheckBoxes) {
		if (!C) continue;
		// https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/15?u=nande

		// the outer hangs to keep us all awake~ (and not get gcd) (doesn't work actually)
		UDelegateWrapper* const Wrapper = NewObject<UDelegateWrapper>(
			this, UDelegateWrapper::StaticClass());
		if (!IsValid(Wrapper)) continue;
		Wrappers.AddUnique(Wrapper); // avoid getting gcd, actually needed
		Wrapper->Obj = C;
		Wrapper->ID = -1;
		Wrapper->OnDispatch.AddUniqueDynamic(this, &UGroupBox::CheckSelected);
		C->OnCheckStateChanged.AddUniqueDynamic(Wrapper, &UDelegateWrapper::DispatchBool);
	}
}

void UGroupBox::NativeDestruct() {
	for (const TObjectPtr<UDelegateWrapper>& W: Wrappers) {
		if (!IsValid(W)) continue;
		W->OnDispatch.RemoveAll(this);
	}
	Wrappers.Empty();
	Super::NativeDestruct();
}

void UGroupBox::CheckSelected(UDelegateWrapper* const W, int32 CID, UObject* const OCB) {
	if (!IsValid(OCB)) return;
	UCheckBox* const CB = static_cast<UCheckBox*>(OCB);
	if (!CB) return;

	const bool IsChecked = CB->IsChecked();
	if (!IsChecked) {
		CB->SetIsChecked(true); // don't allow to manually deselect
	}

	Selected = -1;
	const int32 Num = CheckBoxes.Num();
	for (uint8 i = 0; i<Num; ++i){
		UCheckBox* const C = CheckBoxes[i];
		if (!IsValid(C)) continue;
		
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
