// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Plate.generated.h"

// Base class for Plate. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APlate: public ALInteract {
	GENERATED_BODY()

public:
	APlate();
};
