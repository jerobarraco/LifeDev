// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroUI.h"

#include "MsgBox.h"

void UIntroUI::ShowMsg_Implementation(const FText& Msg) {
	if (!MsgBox) return;
	static TArray<FText> Texts = {
		NSLOCTEXT("Intro", "MsgBox.Btn.OK", "Ok")
	};
	
	MsgBox->Init(Msg, Texts);
	MsgBox->Show();
}

void UIntroUI::MsgDone() {
	if (MsgBox) MsgBox->Hide();
}

void UIntroUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (MsgBox)
		MsgBox->OnDone.AddUniqueDynamic(this, &UIntroUI::MsgDone);
}

void UIntroUI::NativeDestruct() {
	if (MsgBox) MsgBox->OnDone.RemoveAll(this);
	Super::NativeDestruct();
}
