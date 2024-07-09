// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroUI.h"

#include "MsgBox.h"

void UIntroUI::ShowMsg_Implementation(const FText& Msg) {
	if (!MsgBox) return;
	static TArray<FText> Texts = {
		NSLOCTEXT("Intro", "MsgBox.Btn.OK", "Ok")
	};
	
	MsgBox->SetUp(Msg, Texts);
	MsgBox->Show();
	MsgBox->OnDone.AddUniqueDynamic(MsgBox, &UMsgBox::Hide);
}

void UIntroUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
}

void UIntroUI::NativeDestruct() {
	Super::NativeDestruct();
}
