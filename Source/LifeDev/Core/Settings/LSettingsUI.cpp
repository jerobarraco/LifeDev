// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"

#include "GameFramework/GameUserSettings.h"
#include "UI/LQualitySwitchUI.h"

ULSettingsUI::ULSettingsUI():Super() {
	ShowCursor = true;
	
	QSwitchesTexts.Add(ESettingsQuality::GLOBAL_ILLUMINATION,
		FText::FromString(TEXT("Global Illumination")));
}

void ULSettingsUI::NativePreConstruct() {
	Super::NativePreConstruct();

	TArray<ESettingsQuality> OutKeys;
	QSwitchesTexts.GetKeys(OutKeys);
	for (ESettingsQuality Q: OutKeys) {
		const FText* const T = QSwitchesTexts.Find(Q);
		if (!T) continue;
		
		ULQualitySwitchUI** const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;

		(*pSwitchUI)->SetLabel(*T);
	}
}

void ULSettingsUI::LoadQualitySwitch(ESettingsQuality QSwitch) {
	if (QSwitch == ESettingsQuality::NONE) return;
	
	ULQualitySwitchUI** const pSwitchUI = QSwitches.Find(QSwitch);
	
	if (!pSwitchUI || !*pSwitchUI) {
		UE_LOG(LogTemp, Log, TEXT("Cant find quality switc for switch %i"), QSwitch );
		return;
	}

	UGameUserSettings* const Settings = GEngine->GetGameUserSettings();
	if (!Settings) {
		UE_LOG(LogTemp, Warning, TEXT("Can't get user settings"));
		return;
	}

	int32 Q = -1;
	switch(QSwitch) {
	case ESettingsQuality::GLOBAL_ILLUMINATION:
		Q = Settings->GetGlobalIlluminationQuality();
		break;
	default: break;
	}
	(*pSwitchUI)->SetQuality(Q);
}
