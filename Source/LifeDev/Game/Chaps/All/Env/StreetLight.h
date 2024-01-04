// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight02.h"

#include "StreetLight.generated.h"

// Base class for StreetLights
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AStreetLight: public ALLight02 {
	GENERATED_BODY()

public:
	AStreetLight();
};
