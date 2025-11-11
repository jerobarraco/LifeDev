// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetVideoUI.generated.h"

class ULCheck;
class ULFeatsGroup;
class UTextBlock;
class USlider;
class UCheckBox;
class USetAntiAlias;
class UComboBoxString;
class UGroupBox;
enum class EFeat : uint8;

UENUM(Blueprintable)
enum class EQualityType: uint8 {
	NONE,
	OVERALL,
	VIEW_DISTANCE,
	ANTI_ALIAS,
	POST_PROCESS,
	SHADOW,
	GLOBAL_ILLUMINATION,
	REFLECTION,
	TEXTURES,
	EFFECTS,
	FOLIAGE,
	SHADING,
	_MAX UMETA(Hidden)
};

// base class for the video settings
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetVideoUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override;
	virtual void Load_Implementation() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	void RHIsSet();
	void RHIApply() const;
	
	UFUNCTION()
	void FSModeChanged(const FString SelectedItem, const ESelectInfo::Type SelectionType);
	void FSModeSet();
	EWindowMode::Type FSModeGet() const;
	
	void ResSet();
	void ResOptsSet(const bool FullScreen = true);
	static FString ResToCombo(const FIntPoint& P);
	UFUNCTION()
	void ResChanged(const FString SelectedItem, const ESelectInfo::Type SelectionType);
	
	void ResScaleSet();
	UFUNCTION()
	void ResScaleChanged(const float Value);

	void DResSet() const;
	UFUNCTION()
	void DResChanged(const bool bIsChecked);

	void VSyncSet() const;
	UFUNCTION()
	void VSyncChanged(const bool bIsChecked);

	void FrameRateSet() const;
	UFUNCTION() // bound
	void FrameRateChanged(const FString SelectedItem, const ESelectInfo::Type SelectionType);
	void FrameRateApply() const;

	void QSwitchesSet();
	UFUNCTION(BlueprintCallable)
	void QSwitchesLoad() const;
	UFUNCTION(BlueprintCallable)
	void QSwitchLoad(const EQualityType QSwitch) const;
	UFUNCTION() // bound
	void QSwitchChanged(const int32 ID, const int32 NewQ);
	UFUNCTION(BlueprintCallable)
	void QualitySet(const EQualityType Quality, const int32 NewQ);

	void FeatsLoad() const;
	void FeatsApply() const;
	void FeatsSet();

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> Resolution;
	TArray<FIntPoint> ResOpts;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> ResScale;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ResScaleText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> RHIs;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> FSMode;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULCheck> DRes;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULCheck> VSync;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USetAntiAlias> AntiAlias;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> FrameRate;
	static constexpr float FrameRateOpts[] = {0, 15, 20, 25, 30, 60, 75, 90, 120, 144, 150, 180, 210, 240, 360, 420};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULCheck> FRSmooth;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<EQualityType, TObjectPtr<UGroupBox>> QSwitches;
	UPROPERTY(BlueprintReadOnly)
	TMap<EQualityType, FText> QSTexts;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_Overall;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_ViewDistance;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_AntiAlias;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_PostProcess;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_Shadow;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_GI;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_Reflections;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_Textures;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_Effects;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_Foliage;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> QS_Shading;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatsGroup> FeatsGroup;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UGameUserSettings> Settings;
};
