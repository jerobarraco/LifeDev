// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Pencil.generated.h"

// Base class for Pencil. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APencil: public ALInteract {
	GENERATED_BODY()

public:
	APencil();
};
