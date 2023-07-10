// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Clout.generated.h"

// Base class for Clout actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AClout: public ALInteract {
public:
	GENERATED_BODY()

	AClout();
};
