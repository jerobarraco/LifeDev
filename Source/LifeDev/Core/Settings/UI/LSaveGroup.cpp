// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSaveGroup.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Inventory/Flags.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Settings/LSettings.h"

void ULSaveGroup::LoadDoneAll_Implementation() {
	LoadAllSlots = false; // finished

	const bool ShowFoxy = HasDoneSave && ULSettings::GetFeatS(this, EFeat::G_NGP_FOXY);
	const ESlateVisibility Vis = ShowFoxy ? ESlateVisibility::Visible: ESlateVisibility::Collapsed;
	if (LIKELY(SL_Foxy)) SL_Foxy->SetVisibility(Vis);
	if (LIKELY(T_Foxy)) T_Foxy->SetVisibility(Vis);
	
	OnLoadDone.Broadcast(HasDoneSave);
}

void ULSaveGroup::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(SL_Foxy))
		SL_Foxy->OnValueChanged.AddUniqueDynamic(this, &ULSaveGroup::FoxyUpd);
}

void ULSaveGroup::FoxyUpd(const float Value) {
	if (UNLIKELY(!SL_Foxy || !T_Foxy)) return;

	T_Foxy->SetText(
		FText::FromString(FString::Printf(TEXT("Foxify: %.3f"), Value)));
	UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;
	
	Flags->Set(LDConsts::Flags::Settings::Global::Foxy, Value);
	// TODO save foxy value before starting the game
}
