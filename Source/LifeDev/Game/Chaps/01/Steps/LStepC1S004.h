// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S004.generated.h"

class UNiagaraComponent;
// regaining control for player after npc01.
// This finishes when the player receives or picks the card c1
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S004 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S004();

protected:
	virtual void Stop_Implementation() override;
};
