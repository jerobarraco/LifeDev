// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "MsgBox.h"

#include "DelegateWrappers.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMsgBox::NativeOnInitialized() {
	Super::NativeOnInitialized();

	const TArray<UButton*> UBtns = {Btn0, Btn1, Btn2};
	const int32 Num = UBtns.Num();
	for (int32 i=0; i<Num; ++i) {
		UButton* const B = UBtns[i];
		if (!B) continue;

		// the outer hangs to keep us all awake~ (and not get gcd) (doesn't work actually)
		UDelegateWrapper* const Wrapper = NewObject<UDelegateWrapper>(this, UDelegateWrapper::StaticClass());
		if (!IsValid(Wrapper)) continue;

		Wrappers.AddUnique(Wrapper); // avoid getting gcd, actually needed
		Wrapper->Obj = B;
		Wrapper->ID = i;
		Wrapper->OnDispatch.AddUniqueDynamic(this, &UMsgBox::BtnClick);
		B->OnClicked.AddUniqueDynamic(Wrapper, &UDelegateWrapper::Dispatch);
	}	
}

void UMsgBox::NativeDestruct() {
	for (UDelegateWrapper* const W: Wrappers) {
		if (!IsValid(W)) continue;
		W->OnDispatch.RemoveAll(this);
	}
	Wrappers.Empty();

	Super::NativeDestruct();
}
void UMsgBox::Init(const FText& Message, const TArray<FText>& Texts) {
	Msg->SetText(Message);

	const TArray<UButton*> UBtns = {Btn0, Btn1, Btn2};
	const TArray<UTextBlock*> UTexts = {Text0, Text1, Text2};
	const int32 Num = Texts.Num();
	const int32 Num2 = UBtns.Num();
	
	for (int32 i=0; i<Num2; ++i) {
		UButton* const B = UBtns[i];
		if (!B) continue;
		FText T = i < Num ? Texts[i] : FText();
		
		const bool Show = i<Num && !T.IsEmpty();
		B->SetVisibility( Show ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (!Show) continue;
		
		UTextBlock* const Text = UTexts[i];
		if (!Text) continue;
		Text->SetText(T);
	}
}

void UMsgBox::BtnClick(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj) {
	UE_LOG(LogTemp, Log, TEXT("Btn click id=%i"), ID);
	Done(ID);
}
