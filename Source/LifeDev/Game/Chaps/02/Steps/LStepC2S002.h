// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC2S002.generated.h"

class AGhosts;

// step after the boss. will finish if all the items are obtained.
UCLASS(Blueprintable, BlueprintType)
class ALStepC2S002 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC2S002();
};
