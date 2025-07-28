// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetVideoUI.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "GameFramework/GameUserSettings.h"

#include "JUtils/Misc/JUtilsMisc.h"
#include "JUtils/Settings/UI/SetAntiAlias.h"
#include "JUtils/UI/GroupBox.h"

#include "LFeatsGroup.h"
#include "JUtils/Misc/JUtilsSys.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogLSetVid, Log, Log);

void ULSetVideoUI::Apply_Implementation() {
	Super::Apply_Implementation();
	FeatsApply();
	RHIApply();
	if (LIKELY(AntiAlias)) AntiAlias->Apply();
}

void ULSetVideoUI::Load_Implementation() {
	Super::Load_Implementation();
	FeatsLoad();
	FrameRateSet();
	VSyncSet();
	DResSet();
	ResScaleSet();
	ResSet();
	FSModeSet();
	QSwitchesLoad();
	if (LIKELY(AntiAlias)) AntiAlias->Load();
	RHIsSet();
}

void ULSetVideoUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	Settings = GEngine->GetGameUserSettings();

	QSwitchesSet();
	FeatsSet();
	FrameRateSet();
	ResOptsSet();
}

void ULSetVideoUI::NativeDestruct() {
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (EQualityType const Q: Keys) {
		const TObjectPtr<UGroupBox>* const pSwitchUI = QSwitches.Find(Q);
		if (UNLIKELY(!pSwitchUI)) continue;

		(*pSwitchUI)->OnChange.RemoveAll(this);
	}

	if (LIKELY(FrameRate)) FrameRate->ClearOptions();
	if (LIKELY(RHIs)) RHIs->ClearOptions();

	Super::NativeDestruct();
}

void ULSetVideoUI::RHIsSet() {
	if (UNLIKELY(!RHIs)) return;
	
	RHIs->ClearOptions();
	RHIs->OnSelectionChanged.RemoveAll(this);
	for (const EJRHI r : TEnumRange<EJRHI>()) {
		RHIs->AddOption(UEnum::GetValueAsString(r));
	}
	// todo get rhi and set it here
	RHIs->SetSelectedIndex(0);
	// RHIs->OnSelectionChanged.AddUniqueDynamic(this, &ULSetVideoUI::RHIChanged);
}

void ULSetVideoUI::RHIApply() {
	if (UNLIKELY(!RHIs)) return;

	const EJRHI r = EJRHI(RHIs->GetSelectedIndex());
	UE_LOG(LogTemp, Log, TEXT("%hs RHI=%i"), __func__, *UEnum::GetValueAsString(r));
	UJUtilsSys::SetRHI(r);
}

// void ULSetVideoUI::RHIChanged(const FString SelectedItem, const ESelectInfo::Type SelectionType) {
	// UE_LOG(LogTemp, Log, TEXT("%hs Item=%s, Type=%i"), __func__, *SelectedItem, SelectionType);
	// if (UNLIKELY(SelectionType == ESelectInfo::Direct)) return;

// }

void ULSetVideoUI::FSModeSet() {
	const EWindowMode::Type Mode = Settings->GetFullscreenMode();
	if (UNLIKELY(!Mode)) return;

	if (UNLIKELY(!FSMode)) return;
	FSMode->OnSelectionChanged.RemoveAll(this);
	FSMode->ClearOptions();

	// order matters
	// static EWindowMode::Type Modes[] = {
		// EWindowMode::Fullscreen, EWindowMode::WindowedFullscreen, EWindowMode::Windowed};
	static FString Names[] {
		TEXT("Fullscreen"), TEXT("Maximized Window"), TEXT("Windowed")
	};
	constexpr size_t Size = UJUtilsMisc::ArraySize(Names);
	for (size_t i = 0; i < Size; ++i)
		FSMode->AddOption(Names[i]);

	FSMode->SetSelectedIndex(Mode);
	FSMode->OnSelectionChanged.AddUniqueDynamic(this, &ULSetVideoUI::FSModeChanged);
}

void ULSetVideoUI::FSModeChanged(const FString SelectedItem, const ESelectInfo::Type SelectionType) {
	UE_LOG(LogTemp, Log, TEXT("%hs Item=%s, Type=%i"), __func__, *SelectedItem, SelectionType);
	if (UNLIKELY(SelectionType == ESelectInfo::Direct)) return;
	Settings->SetFullscreenMode(static_cast<EWindowMode::Type>(FSMode->GetSelectedIndex()));
}

void ULSetVideoUI::ResSet() const {
	if (UNLIKELY(!Settings | !Resolution)) return;

	Resolution->ClearOptions();
	for (const FIntPoint& P: ResOpts)
		Resolution->AddOption(ResToCombo(P));
	Resolution->SetSelectedOption(ResToCombo(Settings->GetScreenResolution()));
	Resolution->OnSelectionChanged.AddUniqueDynamic(this, &ULSetVideoUI::ResChanged);
}

FString ULSetVideoUI::ResToCombo(const FIntPoint& P) {
	return FString::Printf(TEXT("%ix%i"), P.X, P.Y);
}

void ULSetVideoUI::ResChanged(const FString SelectedItem, const ESelectInfo::Type SelectionType) {
	if (UNLIKELY(!Settings | (SelectionType == ESelectInfo::Type::Direct))) return;

	const int32 Index = Resolution->GetSelectedIndex();
	if (UNLIKELY((Index <0) | (Index > ResOpts.Num()))) return;

	Settings->SetScreenResolution(ResOpts[Index]);
}

void ULSetVideoUI::ResOptsSet() {
	UKismetSystemLibrary::GetConvenientWindowedResolutions(ResOpts);
}

void ULSetVideoUI::ResScaleSet() {
	if (UNLIKELY(!ResScale | !Settings)) return;

	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	ResScale->OnValueChanged.RemoveAll(this);
	const float Value = Settings->GetResolutionScaleNormalized();
	// done this way to trigger the text change. will also re-set the scale but meh.
	ResScale->SetValue(Value);
	ResScaleChanged(Value); // for some extraneous reason it does not trigger the first time
	ResScale->OnValueChanged.AddUniqueDynamic(this, &ULSetVideoUI::ResScaleChanged);
}

void ULSetVideoUI::ResScaleChanged(const float Value) {
	// UE_LOG(LogTemp, Log, TEXT("%hs Val=%.4f"), __func__, Value);
	const int32 PVal = trunc(Value*100);
	if (LIKELY(Settings)) Settings->SetResolutionScaleNormalized(PVal / 100.0);
	if (LIKELY(ResScaleText)) ResScaleText->SetText(
		FText::FromString( FString::FromInt(PVal) + "%" ));
}

void ULSetVideoUI::DResSet() const {
	if (UNLIKELY(!DRes)) return;

	const bool Enabled = Settings ? Settings->IsDynamicResolutionEnabled(): false;
	const ECheckBoxState IsChecked = Enabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	DRes->OnCheckStateChanged.RemoveAll(this); // important or it will change the current
	DRes->SetCheckedState(IsChecked);
	DRes->OnCheckStateChanged.AddUniqueDynamic(this, &ULSetVideoUI::DResChanged); // important or it will change the current
}

void ULSetVideoUI::DResChanged(const bool bIsChecked) {
	if (UNLIKELY(!Settings)) return;

	Settings->SetDynamicResolutionEnabled(bIsChecked);
}

void ULSetVideoUI::VSyncSet() const {
	if (UNLIKELY(!VSync)) return;

	const bool Enabled = Settings ? Settings->IsVSyncEnabled(): false;
	const ECheckBoxState IsChecked = Enabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	VSync->OnCheckStateChanged.RemoveAll(this); // important or it will change the current
	VSync->SetCheckedState(IsChecked);
	VSync->OnCheckStateChanged.AddUniqueDynamic(this, &ULSetVideoUI::VSyncChanged); // important or it will change the current
}

void ULSetVideoUI::VSyncChanged(const bool bIsChecked) {
	if (UNLIKELY(!Settings)) return;

	Settings->SetVSyncEnabled(bIsChecked);
}

void ULSetVideoUI::FrameRateSet() const{
	if (UNLIKELY(!FrameRate)) return;

	const float Current = Settings ? Settings->GetFrameRateLimit() : 0;

	FrameRate->OnSelectionChanged.RemoveAll(this); // important or it will change the current
	FrameRate->ClearOptions();

	int32 CurrentI = 0; // defaults to unlimited
	constexpr size_t N = UJUtilsMisc::ArraySize(FrameRateOpts);
	for (int32 i = 0; i < N; ++i) {
		const float& F = FrameRateOpts[i];
		FrameRate->AddOption(FMath::IsNearlyZero(F) ?
			TEXT("!+UNLIMITED+!") : FString::SanitizeFloat(F, 0));

		if (UNLIKELY(FMath::IsNearlyEqual(Current, F))) CurrentI = i;
	}

	FrameRate->SetSelectedIndex(CurrentI); // set before binding
	FrameRate->OnSelectionChanged.AddUniqueDynamic(this, &ULSetVideoUI::FrameRateChanged);
	
	UE_LOG(LogLSetVid, Log, TEXT("%hs Index=%i Limit=%f"),
		__func__, CurrentI, Current);
}

void ULSetVideoUI::FrameRateChanged(const FString SelectedItem,
	const ESelectInfo::Type SelectionType) {
	if (UNLIKELY(!Settings | !FrameRate)) return;

	// const size_t LimitNum = FrameRateOpts.Num();
	constexpr size_t Num = UJUtilsMisc::ArraySize(FrameRateOpts);

	const int32 Index = FMath::Clamp(FrameRate->GetSelectedIndex(), 0, Num-1);
	Settings->SetFrameRateLimit(FrameRateOpts[Index]);

	UE_LOG(LogLSetVid, Log, TEXT("%hs Num=%i Index=%i Limit=%f"),
		__func__, Num, Index, FrameRateOpts[Index]);
}

void ULSetVideoUI::QSwitchesSet() {
	QSwitches.Add(EQualityType::OVERALL, QS_Overall);
	QSwitches.Add(EQualityType::VIEW_DISTANCE, QS_ViewDistance);
	QSwitches.Add(EQualityType::ANTI_ALIAS, QS_AntiAlias);
	QSwitches.Add(EQualityType::POST_PROCESS, QS_PostProcess);
	QSwitches.Add(EQualityType::SHADOW, QS_Shadow);
	QSwitches.Add(EQualityType::GLOBAL_ILLUMINATION, QS_GI);
	QSwitches.Add(EQualityType::REFLECTION, QS_Reflections);
	QSwitches.Add(EQualityType::TEXTURES, QS_Textures);
	QSwitches.Add(EQualityType::EFFECTS, QS_Effects);
	QSwitches.Add(EQualityType::FOLIAGE, QS_Foliage);
	QSwitches.Add(EQualityType::SHADING, QS_Shading);

	QSTexts.Add(EQualityType::OVERALL,
	NSLOCTEXT("SetVideoUI", "Overall", "Overall"));
	QSTexts.Add(EQualityType::VIEW_DISTANCE,
	NSLOCTEXT("SetVideoUI", "ViewDist", "View Distance"));
	QSTexts.Add(EQualityType::ANTI_ALIAS,
	NSLOCTEXT("SetVideoUI", "AA", "Anti-Aliasing"));
	QSTexts.Add(EQualityType::POST_PROCESS,
	NSLOCTEXT("SetVideoUI", "PP", "Post Processing"));
	QSTexts.Add(EQualityType::SHADOW,
	NSLOCTEXT("SetVideoUI", "Shadows", "Shadows"));
	QSTexts.Add(EQualityType::GLOBAL_ILLUMINATION,
	NSLOCTEXT("SetVideoUI", "GI", "Global Illumination"));
	QSTexts.Add(EQualityType::REFLECTION,
	NSLOCTEXT("SetVideoUI", "Reflections", "Reflections"));
	QSTexts.Add(EQualityType::TEXTURES,
	NSLOCTEXT("SetVideoUI", "Textures", "Textures"));
	QSTexts.Add(EQualityType::EFFECTS,
	NSLOCTEXT("SetVideoUI", "FX", "Effects"));
	QSTexts.Add(EQualityType::FOLIAGE,
	NSLOCTEXT("SetVideoUI", "Foliage", "Foliage"));
	QSTexts.Add(EQualityType::SHADING,
	NSLOCTEXT("SetVideoUI", "Shading", "Shading"));

	UE_LOG(LogLSetVid, Log, TEXT("LSetVideoUI::%hs QTextsN=%i QSwitchesN=%i"),
		__func__, QSTexts.Num(), QSwitches.Num());
	TArray<EQualityType> Keys;
	QSTexts.GetKeys(Keys);
	for (const EQualityType& Q: Keys) {
		const FText* const T = QSTexts.Find(Q);
		if (UNLIKELY(!T)) continue;
		
		TObjectPtr<UGroupBox>* const pSwitchUI = QSwitches.Find(Q);
		if (UNLIKELY(!pSwitchUI)) continue;

		const TObjectPtr<UGroupBox>& SwitchUI = *pSwitchUI;
		SwitchUI->SetLabel(*T);
		SwitchUI->ID = static_cast<int32>(Q);
		SwitchUI->OnChange.AddUniqueDynamic(this, &ULSetVideoUI::QSwitchChanged);
	}
}

void ULSetVideoUI::QSwitchesLoad() const {
	TArray<EQualityType> Keys;
	QSwitches.GetKeys(Keys);
	for (const EQualityType& Q: Keys)
		QSwitchLoad(Q);
}

void ULSetVideoUI::QSwitchLoad(const EQualityType QSwitch) const {
	if (UNLIKELY(QSwitch == EQualityType::NONE)) return;
	
	const TObjectPtr<UGroupBox>* const pSwitchUI = QSwitches.Find(QSwitch);
	if (UNLIKELY(!pSwitchUI || !*pSwitchUI)) { // has to be shortcircuit
		UE_LOG(LogLSetVid, Log, TEXT("Cant find quality switch for %i"), QSwitch);
		return;
	}

	if (UNLIKELY(!Settings)) {
		UE_LOG(LogLSetVid, Warning, TEXT("%hs Can't get user settings"), __func__);
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

void ULSetVideoUI::QualitySet(const EQualityType Quality, const int32 NewQ) {
	UE_LOG(LogLSetVid, Log, TEXT("SetVideoUI: Setting quality=%i newq=%i"), Quality, NewQ);
	if (UNLIKELY(Quality == EQualityType::NONE)) return;

	if (UNLIKELY((NewQ<0) | (NewQ>4))) {
		UE_LOG(LogLSetVid, Warning, TEXT("%hs: New Quality out of bounds quality=%i newq=%i"),
			__func__, Quality, NewQ);
		return;
	}

	if (UNLIKELY(!Settings)) {
		UE_LOG(LogLSetVid, Warning, TEXT("%hs: Can't get user settings"), __func__);
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
		UE_LOG(LogLSetVid, Warning, TEXT("%hs Wrong quality type."), __func__);
		return;
	}
	
	// not optimal but if i set the overall i need to reload the rest and vice versa.
	// so everytime it changes i need to reload. 
	QSwitchesLoad();
}

void ULSetVideoUI::QSwitchChanged(const int32 ID, const int32 NewQ) {
	if (UNLIKELY(
		ID <= static_cast<uint8>(EQualityType::NONE) ||
		ID >= static_cast<uint8>(EQualityType::_MAX))) {
		UE_LOG(LogLSetVid, Warning, TEXT("%hs. Invalid quality id=%i q=%i"), __func__, ID, NewQ);
		return;
	}
	
	const EQualityType K = static_cast<EQualityType>(ID);
	QualitySet(K, NewQ);
}

void ULSetVideoUI::FeatsLoad() const {
	if (LIKELY(FeatsGroup)) FeatsGroup->Load();
}

void ULSetVideoUI::FeatsApply() const {
	if (LIKELY(FeatsGroup)) FeatsGroup->Apply();
}

void ULSetVideoUI::FeatsSet() {
	if (UNLIKELY(!FeatsGroup)) return;

	TMap<EFeat, FText> FeatTexts;
	FeatTexts.Add(EFeat::V_FLASHBACK, NSLOCTEXT("SetVideo", "FFB", "Flashback Post"));
	FeatTexts.Add(EFeat::V_SPEED, NSLOCTEXT("SetVideo", "Speed", "Speed Post"));
	FeatTexts.Add(EFeat::V_STROBE, NSLOCTEXT("SetVideo", "Strobe", "Flashing Lights"));
	FeatTexts.Add(EFeat::V_FOV, NSLOCTEXT("SetVideo", "FOV", "Field of View"));
	FeatTexts.Add(EFeat::V_BLUR, NSLOCTEXT("SetVideo", "Blur", "Motion Blur"));
	FeatTexts.Add(EFeat::V_AUTO_EXP, NSLOCTEXT("SetVideo", "AutoExp", "Auto Exp."));
	FeatTexts.Add(EFeat::V_LUMEN, NSLOCTEXT("SetVideo", "Lumen", "Lumen GI"));
	FeatTexts.Add(EFeat::V_MLIGHTS, NSLOCTEXT("SetVideo", "MLights", "MegaLights"));
	FeatTexts.Add(EFeat::V_NANITE, NSLOCTEXT("SetVideo", "Nanite", "Nanite"));
	FeatsGroup->SetUp(FeatTexts);
}
