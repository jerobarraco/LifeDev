// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC4S006.generated.h"

class ALInteract;

// sponge interact.
UCLASS(Blueprintable, BlueprintType)
class ALStepC4S006: public ALStep {
	GENERATED_BODY()

public:
	ALStepC4S006();
protected:
	virtual void Start_Implementation() override;
};
