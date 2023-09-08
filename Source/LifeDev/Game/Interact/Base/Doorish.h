// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Doorish.generated.h"

// Base class for door-like and drawer actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoorish: public ALInteract {
	GENERATED_BODY()

public:
	ADoorish();
};
