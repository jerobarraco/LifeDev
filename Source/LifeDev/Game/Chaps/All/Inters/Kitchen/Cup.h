// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Cup.generated.h"

// Base class for Cup. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ACup: public ALInteract {
	GENERATED_BODY()

public:
	ACup();
};
