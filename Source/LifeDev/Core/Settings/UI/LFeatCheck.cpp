// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatCheck.h"

#include "Components/TextBlock.h"
#include "LifeDev/Core/Settings/LSettings.h"

ULFeatCheck::ULFeatCheck(const FObjectInitializer& O):Super(O) {}

void ULFeatCheck::SetUp(EFeat NFeat, const FText& NewText) {
	Feat = NFeat;
	UE_LOG(LogTemp, Log, TEXT("LFeatCheck SetUp feat =%i"), Feat);
	if(Text) {
		Text->SetText(NewText);
	}

	Load();
}

void ULFeatCheck::Load() {
	if (!Settings) {
		UE_LOG(LogTemp, Log, TEXT("LFeatCheck.Load Can't find settings."));
		return;
	}

	const bool Enabled = Settings->GetFeat(Feat);
	FeatUpdate(Feat, Enabled);
}

void ULFeatCheck::Apply() {
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

void ULFeatCheck::NativeDestruct() {
	if (Settings) {
		Settings->OnFeatUpdate.RemoveAll(this);
	}
	Settings = nullptr;

	Super::NativeDestruct();
}

void ULFeatCheck::NativeOnInitialized() {
	Super::NativeOnInitialized();
	Settings = GetWorld()->GetGameInstance()->GetSubsystem<ULSettings>();
	if (!Settings) return;
	UE_LOG(LogTemp, Log, TEXT("LFeatCheck NativeInitialized feat =%i"), Feat);

	Settings->OnFeatUpdate.AddUniqueDynamic(this, &ULFeatCheck::FeatUpdate);
}

void ULFeatCheck::FeatUpdate(EFeat NFeat, bool bEnabled) {
	if (!Check) return;
	if (Feat != NFeat) return;
	if (Check->IsChecked() == bEnabled ) return;

	UE_LOG(LogTemp, Log, TEXT("FeatUpdate %i : %i (%s)"),
		Feat, bEnabled, *UEnum::GetValueAsString(Feat));
	const ECheckBoxState NewState = bEnabled ?
		ECheckBoxState::Checked: ECheckBoxState::Unchecked;
	Check->SetCheckedState(NewState);
}
