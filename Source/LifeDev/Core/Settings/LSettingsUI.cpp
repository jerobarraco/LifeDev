// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"

#include "GameFramework/GameUserSettings.h"
#include "JUtils/UI/GroupBox.h"

ULSettingsUI::ULSettingsUI():Super() {
	ShowCursor = true;
	
	QSTexts.Add(EQualityType::GLOBAL_ILLUMINATION,
		FText::FromString(TEXT("Global Illumination")));
}

void ULSettingsUI::NativePreConstruct() {
	Super::NativePreConstruct();

	TArray<EQualityType> Keys;
	QSTexts.GetKeys(Keys);
	for (EQualityType Q: Keys) {
		const FText* const T = QSTexts.Find(Q);
		if (!T) continue;
		
		UGroupBox** const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;

		(*pSwitchUI)->SetLabel(*T);
	}
}

void ULSettingsUI::NativeConstruct() {
	Super::NativeConstruct();
	
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType Q: Keys) {
		UGroupBox** const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;
		(*pSwitchUI)->OnChange.AddUniqueDynamic(this, &ULSettingsUI::QualityChanged);
	}
}

void ULSettingsUI::BeginDestroy() {
	Super::BeginDestroy();
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType Q: Keys) {
		UGroupBox** const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;
		(*pSwitchUI)->OnChange.RemoveAll(this);
	}
}

void ULSettingsUI::LoadQSwitches() {
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType Q: Keys) {
		LoadQSwitch(Q);
	}
}

void ULSettingsUI::LoadQSwitch(EQualityType QSwitch) {
	if (QSwitch == EQualityType::NONE) return;
	
	UGroupBox** const pSwitchUI = QSwitches.Find(QSwitch);
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
	case EQualityType::GLOBAL_ILLUMINATION:
		Q = Settings->GetGlobalIlluminationQuality();
		break;
	default: break;
	}
	(*pSwitchUI)->SetSelected(Q);
}

void ULSettingsUI::SetQuality(EQualityType Quality, int32 NewQ) {
	UE_LOG(LogTemp, Log, TEXT("Setting quality quality=%i newq=%i"), Quality, NewQ);
	if (Quality == EQualityType::NONE) return;
	if (NewQ<0 || NewQ>4) {
		UE_LOG(LogTemp, Warning, TEXT("New Quality out of bounds quality=%i newq=%i"), Quality, NewQ);
		return;
	}
	
	UGameUserSettings* const Settings = GEngine->GetGameUserSettings();
	if (!Settings) {
		UE_LOG(LogTemp, Warning, TEXT("Can't get user settings"));
		return;
	}

	switch (Quality) {
	case EQualityType::GLOBAL_ILLUMINATION:
		Settings->SetGlobalIlluminationQuality(NewQ);
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Wrong quality type"));
	}
}

void ULSettingsUI::QualityChanged(UGroupBox* Group, int32 NewQ) {
	const EQualityType* Key = QSwitches.FindKey(Group);
	if (!Key) {
		UE_LOG(LogTemp, Warning, TEXT("Can't find key for quality switch %lu"), Group);
		return;
	}
	
	SetQuality(*Key, NewQ);
}
