// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "DoorBase.generated.h"

// Base class for door-like and drawer actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoorBase: public ALInteract {
public:
	GENERATED_BODY()

	ADoorBase();
};
