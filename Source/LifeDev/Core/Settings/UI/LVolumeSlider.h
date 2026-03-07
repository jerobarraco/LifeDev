// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "LSlider.h"

#include "LVolumeSlider.generated.h"

// class used to manipulate the sound volume
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULVolumeSlider: public ULSlider {
	GENERATED_BODY()

public:
	ULVolumeSlider();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Preview();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<USoundSubmix> Submix = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FName Key = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool UseAutoPreview = true;

protected:
	virtual void PostInitProperties() override;
	virtual void Apply_Implementation() override;
	virtual void Load_Implementation() override;

	UFUNCTION()
	void ValChanged(const float NVal) { if (UseAutoPreview) Preview(); };
};
