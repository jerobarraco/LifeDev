// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetVideoUI.generated.h"

class UGroupBox;

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

// WIP moving the video stuff from the settingsui
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetVideoUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation(){};

	virtual void Load_Implementation(){};

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable)
	void LoadQSwitches();
	
	UFUNCTION(BlueprintCallable)
	void LoadQSwitch(const EQualityType QSwitch);
	
	UFUNCTION(BlueprintCallable)
	void SetQuality(const EQualityType Quality, const int32 NewQ);

	UFUNCTION() // bound
	void QualityChanged(const int32 ID, const int32 NewQ);
	
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<EQualityType, TObjectPtr<UGroupBox>> QSwitches;

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<EQualityType, FText> QSTexts;
};
