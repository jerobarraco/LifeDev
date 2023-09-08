// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteractFade.h"

#include "Clout.generated.h"

// Base class for Clout actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AClout: public ALInteractFade {
	GENERATED_BODY()

public:
	AClout();
};
