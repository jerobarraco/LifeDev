// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "JUtils/UI/BaseUI.h"

#include "LSettingsUI.generated.h"

class ULQualitySwitchUI;

UENUM(Blueprintable)
enum class ESettingsQuality: uint8 {
	NONE,
	GLOBAL_ILLUMINATION
};

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSettingsUI : public UBaseUI {
	GENERATED_BODY()

public:
	ULSettingsUI();

protected:
	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void LoadQualitySwitch(ESettingsQuality QSwitch);

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<ESettingsQuality, ULQualitySwitchUI*> QSwitches;

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<ESettingsQuality, FText> QSwitchesTexts;
	
};
