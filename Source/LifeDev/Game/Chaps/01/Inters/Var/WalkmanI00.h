// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "WalkmanI00.generated.h"

// there's only one walkman, and i don't foresee.
// using it again, if that happens then i need to create a base class

// Walkman I00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AWalkmanI00: public ALInteract {
	GENERATED_BODY()

public:
	AWalkmanI00();
};
