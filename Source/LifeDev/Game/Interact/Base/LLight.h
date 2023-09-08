// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "LifeDev/Game/Interact/LInteract.h"

#include "LLight.generated.h"

// Base class for LLight actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight: public ALInteract {
	GENERATED_BODY()

public:
	ALLight();
};
