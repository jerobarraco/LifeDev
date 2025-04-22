// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Screwer00.generated.h"

// Base class for Screwer00. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AScrewer00: public ALInteract {
	GENERATED_BODY()

public:
	AScrewer00();
};
