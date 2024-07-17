// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC3S001.generated.h"

class AGhosts;
class ALInteract;

UCLASS(Blueprintable, BlueprintType)
class ALStepC3S001 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC3S001();

protected:
	virtual void TryStart_Implementation() override;
};
