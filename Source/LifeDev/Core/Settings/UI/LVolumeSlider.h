// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "Components/Slider.h"

#include "LVolumeSlider.generated.h"

// class used to manipulate the sound volume
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULVolumeSlider: public USlider {
	GENERATED_BODY()

public:
	ULVolumeSlider();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Preview();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Apply();

	// Loads and applies the value from the flags
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Load();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<USoundSubmix> Submix = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FName Key = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool UseAutoPreview = true;


protected:
	virtual void PostInitProperties() override;
	UFUNCTION()
	void ValChanged(const float NVal) { if (UseAutoPreview) Preview(); };
};
