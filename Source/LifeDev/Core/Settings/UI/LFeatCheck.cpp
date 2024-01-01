// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatCheck.h"

#include "Components/TextBlock.h"
#include "LifeDev/Core/Settings/LSettings.h"

ULFeatCheck::ULFeatCheck(const FObjectInitializer& O):Super(O){}

void ULFeatCheck::SetFeat(EFeat NFeat) {
	Feat = NFeat;
	if(Text) {
		Text->SetDisplayLabel(UEnum::GetValueAsString(Feat));
	}
	if (!Settings) return;

	const bool Enabled = Settings->GetFeat(Feat);
	FeatUpdate(Feat, Enabled);
}


void ULFeatCheck::NativeConstruct() {
	Settings = GetWorld()->GetGameInstance()->GetSubsystem<ULSettings>();
	if (!Settings) return;

	Settings->OnFeatUpdate.AddUniqueDynamic(this, &ULFeatCheck::FeatUpdate);
}

void ULFeatCheck::NativeDestruct() {
	if (!Settings) return;

	Settings->OnFeatUpdate.RemoveAll(this);
	
	Super::NativeDestruct();
}


void ULFeatCheck::FeatUpdate(EFeat NFeat, bool bEnabled) {
	if (Feat != NFeat) return;
	if (!Check) return;

	Check->SetCheckedState(bEnabled ? ECheckBoxState::Checked: ECheckBoxState::Unchecked);
}
