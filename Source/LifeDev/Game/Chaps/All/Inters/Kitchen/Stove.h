// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Stove.generated.h"

// Base class for Stove. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AStove: public ALInteract {
	GENERATED_BODY()

public:
	AStove();
};
