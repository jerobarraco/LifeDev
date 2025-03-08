// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "LOverlayUI.h"

#include "Components/Image.h"
#include "LifeDev/Core/Settings/LSettings.h"

ULOverlayUI::ULOverlayUI() :Super(){
	
}

void ULOverlayUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	ULSettings* const Settings = ULSettings::Instance(this);
	if (UNLIKELY(!Settings)) return;

	Settings->OnSaving.AddUniqueDynamic(this, &ULOverlayUI::SaveUpd);
}


void ULOverlayUI::SaveUpd(const bool NewIsSaving) {
	IsSaving = NewIsSaving;
	
	SaveTimer();
}

void ULOverlayUI::SaveTimer() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	if (!IsSaving) {
		ISaving->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	ISaving->SetVisibility(ESlateVisibility::Visible);

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ULOverlayUI::SaveTimer, 1);
}
