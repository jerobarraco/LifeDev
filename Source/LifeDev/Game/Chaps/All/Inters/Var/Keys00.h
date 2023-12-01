// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Keys00.generated.h"

// Base class for Keys00. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AKeys00: public ALInteract {
	GENERATED_BODY()

public:
	AKeys00();
};
