// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Doors/Door00.h"

#include "DoorI02.generated.h"

// Door from room00 to room01, locked with a key. Instance of the door 00 that is locked until
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoorI02: public ADoor00 {
public:
	GENERATED_BODY()

	ADoorI02();
};
