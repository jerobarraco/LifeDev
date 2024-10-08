// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetVideoUI.generated.h"

class UTextBlock;
class USlider;
class UCheckBox;
class USetAntiAlias;
class UComboBoxString;
class ULFeatCheck;
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

	UFUNCTION()
	void FSModeChanged(const FString SelectedItem, const ESelectInfo::Type SelectionType);
	void FSModeSet();
	void ResSet() const;

	static FString ResToCombo(const FIntPoint& P);
	UFUNCTION()
	void ResChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	void ResOptsSet();
	
	void ResScaleSet() const;
	UFUNCTION()
	void ResScaleChanged(const float Value);
	void DResSet() const;
	UFUNCTION()
	void DResChanged(bool bIsChecked);
	void VSyncSet() const;
	UFUNCTION()
	void VSyncChanged(const bool bIsChecked);
	void FrameRateSet() const;
	UFUNCTION() // bound
	void FrameRateChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

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
	TObjectPtr<UComboBoxString> FSMode;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UCheckBox> DRes;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UCheckBox> VSync;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USetAntiAlias> AntiAlias;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> FrameRate;
	static constexpr float FrameRateOpts[] = {0, 30, 60, 90, 120, 144, 150, 180, 210, 240};

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<EQualityType, TObjectPtr<UGroupBox>> QSwitches;

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<EQualityType, FText> QSTexts;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatCheck> Feat_Speed;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatCheck> Feat_Fov;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatCheck> Feat_Blur;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatCheck> Feat_Strobe;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatCheck> Feat_Lumen;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatCheck> Feat_FBPost;

	UPROPERTY(BlueprintReadOnly)
	TMap<EFeat, TObjectPtr<ULFeatCheck>> Feats;
	UPROPERTY(BlueprintReadOnly)
	TMap<EFeat, FText> FeatTexts;
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<UGameUserSettings> Settings;
};
