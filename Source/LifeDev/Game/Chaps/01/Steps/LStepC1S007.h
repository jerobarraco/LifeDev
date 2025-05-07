// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S007.generated.h"

// npc finished dialog. wait for card and poem.
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S007 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S007();

protected:
	virtual void Start_Implementation() override;
};
