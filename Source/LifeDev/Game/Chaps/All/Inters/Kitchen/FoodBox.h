// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "FoodBox.generated.h"

// Base class for FoodBox. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AFoodBox: public ALInteract {
	GENERATED_BODY()

public:
	AFoodBox();
};
