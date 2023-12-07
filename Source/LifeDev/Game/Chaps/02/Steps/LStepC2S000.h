// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC2S000.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALStepC2S000 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC2S000();
protected:
	void Start_Implementation() override;
};
