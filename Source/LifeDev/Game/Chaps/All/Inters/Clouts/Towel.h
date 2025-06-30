// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Towel.generated.h"

// Base class for Towel actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATowel: public ALInteract {
	GENERATED_BODY()

public:
	ATowel();
};
