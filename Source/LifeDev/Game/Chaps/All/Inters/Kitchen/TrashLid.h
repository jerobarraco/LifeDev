// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "TrashLid.generated.h"

// Base class for TrashLid. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATrashLid: public ALInteract {
	GENERATED_BODY()

public:
	ATrashLid();
};
