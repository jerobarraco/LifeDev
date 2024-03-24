// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "MsgBox.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMsgBox::NativeOnInitialized() {
	Super::NativeOnInitialized();
}

void UMsgBox::ShowButtons(const TArray<FText> Texts) {
	const TArray<UButton*> UBtns = {Btn0, Btn1, Btn2};
	const TArray<UTextBlock*> UTexts = {Text0, Text1, Text2};
	const int32 Num = Texts.Num();
	const int32 Num2 = UBtns.Num();
	
	for (int32 i=0; i<Num2; ++i) {
		UButton* const B = UBtns[i];
		if (!B) continue;
		const FText& T = Texts[i];
		
		const bool Show = i<Num && !T.IsEmpty();
		B->SetVisibility( Show ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (!Show) continue;
		
		UTextBlock* const Text = UTexts[i];
		if (!Text) continue;
		Text->SetText(T);
	}
}
