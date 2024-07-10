// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC4S000.generated.h"

// start of chapter. book ends this
UCLASS(Blueprintable, BlueprintType)
class ALStepC4S000 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC4S000();

protected:
	virtual void TryStart_Implementation() override;
};
