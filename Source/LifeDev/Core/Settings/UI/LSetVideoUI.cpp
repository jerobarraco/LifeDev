// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetVideoUI.h"

#include "GameFramework/GameUserSettings.h"

#include "JUtils/UI/GroupBox.h"

void ULSetVideoUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	QSTexts.Add(EQualityType::OVERALL,
		FText::FromString(TEXT("Overall")));
	QSTexts.Add(EQualityType::VIEW_DISTANCE,
		FText::FromString(TEXT("View Distance")));
	QSTexts.Add(EQualityType::ANTI_ALIAS,
		FText::FromString(TEXT("Anti-Aliasing")));
	QSTexts.Add(EQualityType::POST_PROCESS,
		FText::FromString(TEXT("Post Processing")));
	QSTexts.Add(EQualityType::SHADOW,
		FText::FromString(TEXT("Shadows")));
	QSTexts.Add(EQualityType::GLOBAL_ILLUMINATION,
		FText::FromString(TEXT("Global Illumination")));
	QSTexts.Add(EQualityType::REFLECTION,
		FText::FromString(TEXT("Reflections")));
	QSTexts.Add(EQualityType::TEXTURES,
		FText::FromString(TEXT("Textures")));
	QSTexts.Add(EQualityType::EFFECTS,
		FText::FromString(TEXT("Effects")));
	QSTexts.Add(EQualityType::FOLIAGE,
		FText::FromString(TEXT("Foliage")));
	QSTexts.Add(EQualityType::SHADING,
		FText::FromString(TEXT("Shading")));

	UE_LOG(LogTemp, Log, TEXT("LSetVideoUI::%hs QTextsN=%i QSwitchesN=%i"),
		__func__, QSTexts.Num(), QSwitches.Num());
	TArray<EQualityType> Keys;
	QSTexts.GetKeys(Keys);
	for (EQualityType Q: Keys) {
		const FText* const T = QSTexts.Find(Q);
		if (!T) continue;
		
		UGroupBox** const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;

		UGroupBox* const SwitchUI = *pSwitchUI;
		SwitchUI->SetLabel(*T);
		SwitchUI->ID = static_cast<int32>(Q);
		SwitchUI->OnChange.AddUniqueDynamic(this, &ULSetVideoUI::QualityChanged);
	}
}

void ULSetVideoUI::NativeDestruct() {
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType const Q: Keys) {
		UGroupBox** const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;

		(*pSwitchUI)->OnChange.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void ULSetVideoUI::LoadQSwitches() {
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType const Q: Keys) {
		LoadQSwitch(Q);
	}
}

void ULSetVideoUI::LoadQSwitch(EQualityType QSwitch) {
	if (QSwitch == EQualityType::NONE) return;
	
	UGroupBox** const pSwitchUI = QSwitches.Find(QSwitch);
	if (!pSwitchUI || !*pSwitchUI) {
		UE_LOG(LogTemp, Log, TEXT("Cant find quality switch for %i"), QSwitch);
		return;
	}

	const UGameUserSettings* const Settings = GEngine->GetGameUserSettings();
	if (!Settings) {
		UE_LOG(LogTemp, Warning, TEXT("Can't get user settings"));
		return;
	}

	int32 Q = -1;
	switch(QSwitch) {
	case EQualityType::OVERALL:
		Q = Settings->GetOverallScalabilityLevel();
		break;
	case EQualityType::VIEW_DISTANCE:
		Q = Settings->GetViewDistanceQuality();
		break;
	case EQualityType::ANTI_ALIAS:
		Q = Settings->GetAntiAliasingQuality();
		break;
	case EQualityType::POST_PROCESS:
		Q = Settings->GetPostProcessingQuality();
		break;
	case EQualityType::SHADOW:
		Q = Settings->GetShadowQuality();
		break;
	case EQualityType::GLOBAL_ILLUMINATION:
		Q = Settings->GetGlobalIlluminationQuality();
		break;
	case EQualityType::REFLECTION:
		Q = Settings->GetReflectionQuality();
		break;
	case EQualityType::TEXTURES:
		Q = Settings->GetTextureQuality();
		break;
	case EQualityType::EFFECTS:
		Q = Settings->GetVisualEffectQuality();
		break;
	case EQualityType::FOLIAGE:
		Q = Settings->GetFoliageQuality();
		break;
	case EQualityType::SHADING:
		Q = Settings->GetShadingQuality();
		break;
	default:
		return; // don't select anything.
	}
	(*pSwitchUI)->SetSelected(Q);
}

void ULSetVideoUI::SetQuality(EQualityType Quality, int32 NewQ) {
	UE_LOG(LogTemp, Log, TEXT("SetVideoUI: Setting quality=%i newq=%i"), Quality, NewQ);
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
	case EQualityType::OVERALL:
		Settings->SetOverallScalabilityLevel(NewQ);
		break;
	case EQualityType::VIEW_DISTANCE:
		Settings->SetViewDistanceQuality(NewQ);
		break;
	case EQualityType::ANTI_ALIAS:
		Settings->SetAntiAliasingQuality(NewQ);
		break;
	case EQualityType::POST_PROCESS:
		Settings->SetPostProcessingQuality(NewQ);
		break;
	case EQualityType::SHADOW:
		Settings->SetShadowQuality(NewQ);
		break;
	case EQualityType::GLOBAL_ILLUMINATION:
		Settings->SetGlobalIlluminationQuality(NewQ);
		break;
	case EQualityType::REFLECTION:
		Settings->SetReflectionQuality(NewQ);
		break;
	case EQualityType::TEXTURES:
		Settings->SetTextureQuality(NewQ);
		break;
	case EQualityType::EFFECTS:
		Settings->SetVisualEffectQuality(NewQ);
		break;
	case EQualityType::FOLIAGE:
		Settings->SetFoliageQuality(NewQ);
		break;
	case EQualityType::SHADING:
		Settings->SetShadingQuality(NewQ);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Wrong quality type."));
		return;
	}
	
	// not optimal but if i set the overall i need to reload the rest and vice versa.
	// so everytime it changes i need to reload. 
	LoadQSwitches();
}

void ULSetVideoUI::QualityChanged(int32 ID, int32 NewQ) {
	if (ID <= static_cast<uint8>(EQualityType::NONE) || ID >= static_cast<uint8>(EQualityType::_MAX)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs. Invalid quality id=%i q=%i"), __func__, ID, NewQ);
		return;
	}
	
	const EQualityType K = static_cast<EQualityType>(ID);
	SetQuality(K, NewQ);
}

// lumen is disabled by the feat in the featsman
