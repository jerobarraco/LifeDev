// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Doors/Door00.h"

#include "DoorI02.generated.h"

// Door from room01 to hall00. locked in chap01
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoorI02: public ADoor00 {
	GENERATED_BODY()

public:
	ADoorI02();
};
