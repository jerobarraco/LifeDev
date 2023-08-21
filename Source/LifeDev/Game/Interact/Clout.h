// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LInteractFade.h"

#include "Clout.generated.h"

// Base class for Clout actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AClout: public ALInteractFade {
public:
	GENERATED_BODY()

	AClout();
};
