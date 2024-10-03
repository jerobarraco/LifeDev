// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetVideoUI.h"

#include "LFeatCheck.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

#include "JUtils/UI/GroupBox.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

void ULSetVideoUI::Apply_Implementation() {
	Super::Apply_Implementation();
	FeatsApply();
}

void ULSetVideoUI::Load_Implementation() {
	Super::Load_Implementation();
	FeatsLoad();
}

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
		
		TObjectPtr<UGroupBox>* const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;

		const TObjectPtr<UGroupBox>& SwitchUI = *pSwitchUI;
		SwitchUI->SetLabel(*T);
		SwitchUI->ID = static_cast<int32>(Q);
		SwitchUI->OnChange.AddUniqueDynamic(this, &ULSetVideoUI::QualityChanged);
	}

	FeatsSet();
	FrameRateSet();
}

void ULSetVideoUI::NativeDestruct() {
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType const Q: Keys) {
		TObjectPtr<UGroupBox>* const pSwitchUI = QSwitches.Find(Q);
		if (!pSwitchUI) continue;

		(*pSwitchUI)->OnChange.RemoveAll(this);
	}

	Feats.Empty(0);
	FeatTexts.Empty();
	if (FrameRate) FrameRate->ClearOptions();
	Super::NativeDestruct();
}

void ULSetVideoUI::FrameRateSet() {
	if (!FrameRate) [[unlikely]] return;
	FrameRate->ClearOptions();
	UGameUserSettings* const UserSettings = GEngine->GetGameUserSettings();
	const float Current = UserSettings? UserSettings->GetFrameRateLimit() : 0;
	int32 CurrentI = 0;
	for (int32 i= 0; i< FrameRateOpts.Num(); ++i) {
		const float& F= FrameRateOpts[i];
		FrameRate->AddOption(FMath::IsNearlyZero(F) ? TEXT("!!UNLIMITED!!") : TEXT("30"));
		if (FMath::IsNearlyEqual(Current, F)) CurrentI = i;
	}
	FrameRate->SetSelectedIndex(CurrentI);
	FrameRate->OnSelectionChanged.AddUniqueDynamic(this, &ULSetVideoUI::FrameRateChanged);
}

void ULSetVideoUI::FrameRateChanged(FString const SelectedItem, ESelectInfo::Type const SelectionType) {
	UGameUserSettings* const UserSettings = GEngine->GetGameUserSettings();
	if (!UserSettings || !FrameRate) [[unlikely]] return;

	const size_t LimitNum = FrameRateOpts.Num();
	const int32 Index = FMath::Clamp(FrameRate->GetSelectedIndex(), 0, LimitNum-1);
	UserSettings->SetFrameRateLimit(FrameRateOpts[Index]);

	UE_LOG(LogTemp, Log, TEXT("%hs LimitNum=%i Index=%i Limit=%f"),
		__func__, LimitNum, Index, FrameRateOpts[Index]);
}

void ULSetVideoUI::LoadQSwitches() {
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType const Q: Keys) {
		LoadQSwitch(Q);
	}
}

void ULSetVideoUI::LoadQSwitch(const EQualityType QSwitch) {
	if (QSwitch == EQualityType::NONE) return;
	
	TObjectPtr<UGroupBox>* const pSwitchUI = QSwitches.Find(QSwitch);
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

void ULSetVideoUI::SetQuality(const EQualityType Quality, const int32 NewQ) {
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
		UE_LOG(LogTemp, Warning, TEXT("%hs Wrong quality type."), __func__);
		return;
	}
	
	// not optimal but if i set the overall i need to reload the rest and vice versa.
	// so everytime it changes i need to reload. 
	LoadQSwitches();
}

void ULSetVideoUI::QualityChanged(const int32 ID, const int32 NewQ) {
	if (ID <= static_cast<uint8>(EQualityType::NONE) || ID >= static_cast<uint8>(EQualityType::_MAX)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs. Invalid quality id=%i q=%i"), __func__, ID, NewQ);
		return;
	}
	
	const EQualityType K = static_cast<EQualityType>(ID);
	SetQuality(K, NewQ);
}

void ULSetVideoUI::FeatsLoad() const {
	for (const auto& KV: Feats) {
		const TObjectPtr<ULFeatCheck>& F = KV.Value;
		if (!F) continue;

		F->Load();
	}
}

void ULSetVideoUI::FeatsApply() const {
	for (const auto& KV: Feats) {
		const TObjectPtr<ULFeatCheck>& F = KV.Value;
		if (!F) continue;

		F->Apply();
	}
}

void ULSetVideoUI::FeatsSet() {
	Feats.Empty(6);
	FeatTexts.Empty(6);

	Feats.Add(EFeat::V_BLUR, Feat_Blur);
	Feats.Add(EFeat::V_SPEED, Feat_Speed);
	Feats.Add(EFeat::V_LUMEN, Feat_Lumen);
	Feats.Add(EFeat::V_FLASHBACK, Feat_FBPost);
	Feats.Add(EFeat::V_STROBE, Feat_Strobe);
	Feats.Add(EFeat::V_FOV, Feat_Fov);

	FeatTexts.Add(EFeat::V_FLASHBACK, NSLOCTEXT("SetVideo", "FFB", "Flashback Post"));
	FeatTexts.Add(EFeat::V_SPEED, NSLOCTEXT("SetVideo", "Speed", "Speed Post"));
	FeatTexts.Add(EFeat::V_FOV, NSLOCTEXT("SetVideo", "FOV", "Field of View"));
	FeatTexts.Add(EFeat::V_BLUR, NSLOCTEXT("SetVideo", "Blur", "Motion Blur"));
	FeatTexts.Add(EFeat::V_LUMEN, NSLOCTEXT("SetVideo", "Lumen", "Lumen GI"));
	FeatTexts.Add(EFeat::V_STROBE, NSLOCTEXT("SetVideo", "Strobe", "Flashing Lights"));
	
	for (const auto& KV: Feats) {
		const EFeat K = KV.Key;
		const TObjectPtr<ULFeatCheck>& F = KV.Value;
		if (!F) continue;

		const FText* const T = FeatTexts.Find(K);
		if (T) F->SetUp(K, *T);
	}
}

// lumen is disabled by the feat in the featsman
