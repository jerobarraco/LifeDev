// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatCheck.h"

#include "Components/TextBlock.h"
#include "LifeDev/Core/Settings/LSettings.h"

ULFeatCheck::ULFeatCheck(const FObjectInitializer& O):Super(O){}

void ULFeatCheck::SetFeat(EFeat NFeat) {
	Feat = NFeat;
	UE_LOG(LogTemp, Log, TEXT("LFeatCheck SetFeat feat =%i"), Feat);
	if(Text) {
		Text->SetText(FText::FromString(UEnum::GetValueAsString(Feat)));
	}
	if (!Settings) {
		UE_LOG(LogTemp, Log, TEXT("LFeatCheck can't find settings"));
		return;
	}

	const bool Enabled = Settings->GetFeat(Feat);
	FeatUpdate(Feat, Enabled);
}

void ULFeatCheck::NativeDestruct() {
	if (!Settings) {
		Settings->OnFeatUpdate.RemoveAll(this);
	}

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
	UE_LOG(LogTemp, Log, TEXT("FeatUpdate %i : %i"), NFeat, bEnabled);
	if (Feat != NFeat) return;
	if (!Check) return;

	Check->SetCheckedState(bEnabled ? ECheckBoxState::Checked: ECheckBoxState::Unchecked);
}
