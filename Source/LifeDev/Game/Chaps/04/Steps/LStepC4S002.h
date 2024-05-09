// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC4S002.generated.h"

class ALInteract;

// pot and stove interactions
UCLASS(Blueprintable, BlueprintType)
class ALStepC4S002 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC4S002();
protected:
	virtual void Start_Implementation() override;
};
