// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "JSlider.h"

#include "LSlider.generated.h"

// base class for sliders. implements load/save and style
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSlider : public UJSlider {
	GENERATED_BODY()
};
