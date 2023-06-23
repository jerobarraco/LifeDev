// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S001.generated.h"

// main game step
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S001 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S001();

protected:
	virtual void Start_Implementation() override;
	virtual void Debug_Implementation() override;
};
