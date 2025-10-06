// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatCheck.h"

#include "Components/CheckBox.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

void ULFeatCheck::SetUp(const EFeat NFeat, const FText& NewText) {
	Feat = NFeat;
	UE_LOG(LogTemp, Log, TEXT("LFeatCheck::Setup feat=%s"), *UEnum::GetValueAsString(Feat));
	// if (LIKELY(Text)) Text->SetText(NewText); // not necessary
	Label = NewText;
	Load();
}

void ULFeatCheck::Load_Implementation() {
	if (!Settings) {
		UE_LOG(LogTemp, Log, TEXT("LFeatCheck.Load Can't find settings. feat=%s"),
			*UEnum::GetValueAsString(Feat));
		return;
	}

	WasEnabled = Settings->GetFeat(Feat);
	FeatUpdate(Feat, WasEnabled);
}

void ULFeatCheck::Apply_Implementation() {
	if (!Settings) {
		UE_LOG(LogTemp, Log, TEXT("LFeatCheck.Apply Can't find settings."));
		return;
	}
	if (!Check) {
		UE_LOG(LogTemp, Log, TEXT("LFeatCheck.Apply Can't find check."));
		return;
	}
	
	Settings->SetFeat(Feat, Check->IsChecked());
}

void ULFeatCheck::SetDefault() {
	const ULSysSettings* const Settings = ULSysSettings::Get();
	if (UNLIKELY(!Settings)) return;

	const TSet<EFeat>& Feats = Settings->GetFeats();
	FeatUpdate(Feat, Feats.Contains(Feat));
}

void ULFeatCheck::NativeDestruct() {
	if (LIKELY(Settings)) Settings->OnFeatUpdate.RemoveAll(this);
	Settings = nullptr;

	Super::NativeDestruct();
}

void ULFeatCheck::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	Settings = ULSettings::Instance(this);
	if (UNLIKELY(!Settings | !Check)) return;

	UE_LOG(LogTemp, Log, TEXT("LFeatCheck NativeInitialized feat=%s"),
		*UEnum::GetValueAsString(Feat));

	Settings->OnFeatUpdate.AddUniqueDynamic(this, &ULFeatCheck::FeatUpdate);
	Check->OnCheckStateChanged.AddUniqueDynamic(this, &ULFeatCheck::CheckChanged);
}

void ULFeatCheck::FeatUpdate(const EFeat NFeat, const bool bEnabled) {
	if (UNLIKELY(!Check)) return;
	if (Feat != NFeat) return;
	// avoid infinite loop and stack overflow with auto apply (it's also more efficient).
	if (Check->IsChecked() == bEnabled) return;

	UE_LOG(LogTemp, Log, TEXT("FeatUpdate %i : %i (%s)"),
		Feat, bEnabled, *UEnum::GetValueAsString(Feat));
	const ECheckBoxState NewState = bEnabled ?
		ECheckBoxState::Checked: ECheckBoxState::Unchecked;
	Check->SetCheckedState(NewState);
	// don't update WasEnabled here. since it could be triggered by the set itself.
}

void ULFeatCheck::CheckChanged(const bool bEnabled) {
	UE_LOG(LogTemp, Log, TEXT("%hs %i : %i (%s)"),
		__func__, Feat, bEnabled, *UEnum::GetValueAsString(Feat));
	// notice this could trigger an infinite loop. But the IsChecked == bEnabled protects us 
	if (AutoApply) Apply();
}
