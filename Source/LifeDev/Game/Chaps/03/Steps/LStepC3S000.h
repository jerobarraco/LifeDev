// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC3S000.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALStepC3S000 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC3S000();

protected:
	virtual void TryStart_Implementation() override;
	virtual void DoDebug_Implementation() override;
};
