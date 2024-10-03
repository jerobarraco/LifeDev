// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "JUtils/UI/BaseUI.h"

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
class LIFEDEV_API ULSetVideoUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction))
	void Apply();
	virtual void Apply_Implementation(){};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction))
	void Load();
	virtual void Load_Implementation(){};

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable)
	void LoadQSwitches();
	
	UFUNCTION(BlueprintCallable)
	void LoadQSwitch(EQualityType QSwitch);
	
	UFUNCTION(BlueprintCallable)
	void SetQuality(EQualityType Quality, int32 NewQ);

	UFUNCTION() // bound
	void QualityChanged(int32 ID, int32 NewQ);
	
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<EQualityType, TObjectPtr<UGroupBox>> QSwitches;

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TMap<EQualityType, FText> QSTexts;
};
