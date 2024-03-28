// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Sponge.generated.h"

// Base class for Sponge. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ASponge: public ALInteract {
	GENERATED_BODY()

public:
	ASponge();
};
