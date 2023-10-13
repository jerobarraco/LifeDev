// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S000.generated.h"

// chapter 1 start
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S000 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S000();

protected:
	virtual void Start_Implementation() override;
	virtual void Debug_Implementation() override;
};
