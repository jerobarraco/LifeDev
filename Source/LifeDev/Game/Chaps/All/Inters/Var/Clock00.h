// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Clock00.generated.h"

// Base class for Clock00. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AClock00: public ALInteract {
	GENERATED_BODY()

public:
	AClock00();
};
