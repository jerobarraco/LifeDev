// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S004.generated.h"

class UNiagaraComponent;
// regaining control for player after npc01.
// NPC01 will trigger step finish on rewarding the card, or maybe i can put the itemfinish as the card
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S004 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S004();
};
