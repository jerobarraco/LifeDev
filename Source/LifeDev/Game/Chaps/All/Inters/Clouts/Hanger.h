// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Hanger.generated.h"

// Base class for Hanger actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AHanger: public ALInteract {
	GENERATED_BODY()

public:
	AHanger();
};
