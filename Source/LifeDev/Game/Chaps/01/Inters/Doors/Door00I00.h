// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Doors/Door00.h"

#include "Door00I00.generated.h"

// Door from room00 to room01, locked with a key. Instance of the door 00 that is locked until
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor00I00: public ADoor00 {
public:
	GENERATED_BODY()

	ADoor00I00();
};
