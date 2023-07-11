// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Door.h"

#include "Door01.generated.h"

// TODO make a base class for this
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor01: public ADoor {
public:
	GENERATED_BODY()

	ADoor01();
};
