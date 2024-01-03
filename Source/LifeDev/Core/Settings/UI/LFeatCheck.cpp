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
	if (!Settings) {
		UE_LOG(LogTemp, Log, TEXT("LFeatCheck can't find settings"));
		return;
	}

	const bool Enabled = Settings->GetFeat(Feat);
	FeatUpdate(Feat, Enabled);
}

void ULFeatCheck::NativeDestruct() {
	if (Settings) {
		Settings->OnFeatUpdate.RemoveAll(this);
	}
	Settings = nullptr;

	if (Check) {
		Check->OnCheckStateChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void ULFeatCheck::NativeOnInitialized() {
	Super::NativeOnInitialized();
	Settings = GetWorld()->GetGameInstance()->GetSubsystem<ULSettings>();
	if (!Settings) return;
	UE_LOG(LogTemp, Log, TEXT("LFeatCheck NativeInitialized feat =%i"), Feat);

	Settings->OnFeatUpdate.AddUniqueDynamic(this, &ULFeatCheck::FeatUpdate);

	if (!Check) return;
	Check->OnCheckStateChanged.AddUniqueDynamic(this, &ULFeatCheck::CheckUpdate);
}

void ULFeatCheck::CheckUpdate(bool bIsChecked) {
	if (!Settings) return;
	Settings->SetFeat(Feat, bIsChecked);
}

void ULFeatCheck::FeatUpdate(EFeat NFeat, bool bEnabled) {
	if (!Check) return;
	if (Feat != NFeat) return;

	UE_LOG(LogTemp, Log, TEXT("FeatUpdate %i : %i (%s)"),
		Feat, bEnabled, *UEnum::GetValueAsString(Feat));
	Check->SetCheckedState(bEnabled ? ECheckBoxState::Checked: ECheckBoxState::Unchecked);
}
