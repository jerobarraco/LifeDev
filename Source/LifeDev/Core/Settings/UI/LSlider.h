// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"

#include "LSlider.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSlider : public USlider {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Apply() {};

	// Loads and applies the value from the flags
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Load() {};
};
