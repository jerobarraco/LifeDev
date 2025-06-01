// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroUI.h"

#include "Components/WidgetSwitcher.h"

#include "JButton.h"
#include "MsgBox.h"

void UIntroUI::ShowMsg_Implementation(const FText& Msg) {
	if (UNLIKELY(!MsgBox)) return;
	static TArray<FText> Texts = {
		NSLOCTEXT("Intro", "MsgBox.Btn.OK", "Ok")
	};
	
	MsgBox->SetUp(Msg, Texts);
	MsgBox->Show();
}

// implemented on cpp
void UIntroUI::ShowSettings_Implementation(const int32 Id) {}

// post init, and postLoad doesn't work to set up the buttons :(
// constructor might be too early.
void UIntroUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (UNLIKELY(!Switcher)) return;
	if (LIKELY(BtnNext))
		BtnNext->SetUp(NSLOCTEXT("Intro", "BtnNext", "Continue"), 1);
	if (LIKELY(BtnNext2))
		BtnNext2->SetUp(NSLOCTEXT("Intro", "BtnNext2", "Ok"), 2);
	if (LIKELY(BtnSettings))
		BtnSettings->SetUp(NSLOCTEXT("Intro", "BtnSettings", "Settings"), -1);
	if (LIKELY(BtnDone))
		BtnDone->SetUp(NSLOCTEXT("Intro", "BtnStart", "Start"), -1);
	if (LIKELY(BtnNext)) BtnNext->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	if (LIKELY(BtnNext2)) BtnNext2->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	if (LIKELY(BtnSettings)) BtnSettings->OnClick.AddUniqueDynamic(this, &UIntroUI::ShowSettings);
	if (LIKELY(BtnDone)) BtnDone->OnClick.AddUniqueDynamic(this, &UIntroUI::Done);
}

void UIntroUI::NativeDestruct() {
	Super::NativeDestruct();
}

void UIntroUI::SlotsLoadDone(const bool HasDoneSave) {
	OnSlotsDone.Broadcast(HasDoneSave);
}
