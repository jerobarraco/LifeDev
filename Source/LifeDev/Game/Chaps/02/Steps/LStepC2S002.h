// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC2S002.generated.h"

// step after the boss. waits for tape, that's all it does.
UCLASS(Blueprintable, BlueprintType)
class ALStepC2S002 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC2S002();
};
