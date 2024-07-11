// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "Components/Slider.h"

#include "LSoundSlider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJSoundSliderDone, float, Val);

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSoundSlider: public USlider {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Apply();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Save();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Reset(){};

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Load(){};

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	USoundSubmix* Submix = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FName Key = NAME_None;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient, Category=SetUp)
	FJSoundSliderDone OnDone;
};
