// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSaveGroup.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"

#include "JButton.h"
#include "Inventory/Flags.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Settings/LSettings.h"

void ULSaveGroup::LoadDoneAll_Implementation() {
	LoadAllSlots = false; // finished

	const bool ShowFoxy = HasDoneSave && ULSettings::GetFeatS(this, EFeat::G_NGP_FOXY);
	const ESlateVisibility Vis = ShowFoxy ? ESlateVisibility::Visible: ESlateVisibility::Collapsed;
	if (LIKELY(SLFoxy)) SLFoxy->SetVisibility(Vis);
	if (LIKELY(TFoxy)) TFoxy->SetVisibility(Vis);
	
	if (LIKELY(BtnSettings)) {
		BtnSettings->SetUp(NSLOCTEXT("Intro", "BtnSettings", "Settings"), -1);
		BtnSettings->OnClick.AddUniqueDynamic(this, &ULSaveGroup::DoSettings);
	}
	if (LIKELY(BtnStart)) {
		BtnStart->SetUp(NSLOCTEXT("Intro", "BtnStart", "Start"), -1);
		BtnStart->OnClick.AddUniqueDynamic(this, &ULSaveGroup::DoStart);
	}

	if (LIKELY(BtnBack))
		BtnBack->OnClick.AddUniqueDynamic(this, &ULSaveGroup::DoBack);

	OnLoadDone.Broadcast(HasDoneSave);
}

// this is bound to IntroUI::Done, 0 means start
void ULSaveGroup::DoStart(const int32 pId) { OnDone.Broadcast(0); }
void ULSaveGroup::DoSettings(const int32 pId) { OnSettings.Broadcast(); }
void ULSaveGroup::DoBack(const int32 pId) { OnBack.Broadcast(); }

void ULSaveGroup::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(SLFoxy))
		SLFoxy->OnValueChanged.AddUniqueDynamic(this, &ULSaveGroup::FoxyUpd);
}

void ULSaveGroup::NativeDestruct() {
	if (LIKELY(SLFoxy)) SLFoxy->OnValueChanged.RemoveAll(this);
	if (LIKELY(BtnSettings)) BtnSettings->OnClick.RemoveAll(this);
	if (LIKELY(BtnStart)) BtnStart->OnClick.RemoveAll(this);
	if (LIKELY(BtnBack)) BtnBack->OnClick.RemoveAll(this);
	Super::NativeDestruct();
}

void ULSaveGroup::FoxyUpd(const float Value) {
	if (UNLIKELY(!SLFoxy | !TFoxy)) return;

	TFoxy->SetText(FText::FromString(FString::Printf(TEXT("Foxy: %.3f"), Value)));
	UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;

	Flags->Set(LDConsts::Flags::Settings::Global::Foxy, Value);
}
