// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "MsgBox.h"

#include "JButton.h"
#include "Components/TextBlock.h"

void UMsgBox::NativeOnInitialized() {
	Super::NativeOnInitialized();

	const TArray<UJButton*> UBtns = {Btn0, Btn1, Btn2};
	const int32 Num = UBtns.Num();
	for (int32 i=0; i<Num; ++i) {
		UJButton* const B = UBtns[i];
		if (!B) continue;
		B->OnClick.AddUniqueDynamic(this, &UMsgBox::BtnClick);
	}	
}

void UMsgBox::NativeDestruct() {
	for (UJButton* const B: {Btn0, Btn1, Btn2}) {
		if (!B) continue;
		B->OnClick.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UMsgBox::Init(const FText& Message, const TArray<FText>& Texts) {
	Msg->SetText(Message);

	const TArray<UJButton*> UBtns = {Btn0, Btn1, Btn2};
	const int32 Num = Texts.Num();
	const int32 Num2 = UBtns.Num();
	
	for (int32 i=0; i<Num2; ++i) {
		UJButton* const B = UBtns[i];
		if (!B) continue;

		const FText& T = i < Num ? Texts[i] : FText();
		const bool Show = i<Num && !T.IsEmpty();
		B->SetVisibility( Show ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (!Show) continue;
		B->SetUp(Texts[i], i);
	}
}

void UMsgBox::BtnClick(int32 ID) {
	UE_LOG(LogTemp, Log, TEXT("Btn click id=%i"), ID);
	Done(ID);
}
