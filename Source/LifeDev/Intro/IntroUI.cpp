// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroUI.h"

#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"

#include "LifeDev/Core/Settings/UI/LSaveGroup.h"
#include "JButton.h"
#include "MsgBox.h"

void UIntroUI::ShowMsg_Implementation(const FText& Msg, const TArray<FText>& NewBtns) {
	if (UNLIKELY(!MsgBox)) return;
	static TArray<FText> DefaultBtns = { NSLOCTEXT("Intro", "MsgBox.Btn.OK", "Ok") };
	const TArray<FText>& Btns = NewBtns.Num() >0 ? NewBtns : DefaultBtns;  
	MsgBox->SetUp(Msg, Btns);
	MsgBox->Show();
}

void UIntroUI::ShowSettings_Implementation() {}

// post init, and postLoad doesn't work to set up the buttons :(
// constructor might be too early.
void UIntroUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (UNLIKELY(!Switcher)) return;
	if (LIKELY(BtnNext)) {
		BtnNext->SetUp(NSLOCTEXT("Intro", "BtnNext", "Ok"), 1);
		BtnNext->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	}
	if (LIKELY(BtnBack)) {
		BtnBack->SetUp(NSLOCTEXT("Intro", "BtnBack", "Back"), 0);
		BtnBack->OnClick.AddUniqueDynamic(Switcher, &UWidgetSwitcher::SetActiveWidgetIndex);
	}
	if (LIKELY(BtnSettings)) {
		BtnSettings->SetUp(NSLOCTEXT("Intro", "BtnSettings", "Settings"), -1);
		BtnSettings->OnClick.AddUniqueDynamic(this, &UIntroUI::DoSettings);
	}
	if (LIKELY(BtnDone)) {
		BtnDone->SetUp(NSLOCTEXT("Intro", "BtnDone", "Start"), -1);
		BtnDone->OnClick.AddUniqueDynamic(this, &UIntroUI::Done);
	}

	if (LIKELY(BtnQuit)) {
		BtnQuit->SetUp(NSLOCTEXT("Intro", "BtnQuit", "Quit"), -1);
		BtnQuit->OnClick.AddUniqueDynamic(this, &UIntroUI::DoQuit);
	}

	if (LIKELY(SaveGroup)) {
		SaveGroup->OnDone.AddUniqueDynamic(this, &UIntroUI::Done); // todo remove
		SaveGroup->OnSettings.AddUniqueDynamic(this, &UIntroUI::ShowSettings); // todo remove
		// SaveGroup->OnBack // todo remove 
	}
}

void UIntroUI::NativeDestruct() {
	Super::NativeDestruct();
}

void UIntroUI::DoQuit(const int32 Id) {
	ShowMsg(NSLOCTEXT("Intro", "QuitPrompt", "Are you sure?"), {}); // todo add buttons and hook to callback

	UKismetSystemLibrary::QuitGame(this, nullptr,
		EQuitPreference::Quit, false);
}

void UIntroUI::SlotsLoadDone(const bool HasDoneSave) {
	OnSlotsDone.Broadcast(HasDoneSave);
}
