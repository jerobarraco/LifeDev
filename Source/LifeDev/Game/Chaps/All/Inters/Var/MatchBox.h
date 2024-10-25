// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "MatchBox.generated.h"

// Base class for MatchBox. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AMatchBox: public ALInteract {
	GENERATED_BODY()

public:
	AMatchBox();
};
