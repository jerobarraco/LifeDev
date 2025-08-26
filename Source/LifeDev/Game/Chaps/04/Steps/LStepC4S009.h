// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC4S009.generated.h"

// some dialog. between plate/sit and end.
UCLASS(Blueprintable, BlueprintType)
class ALStepC4S009 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC4S009();

protected:
	virtual void Start_Implementation() override;
};
