// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Kitchen/Fridge.h"

#include "FridgeI00.generated.h"

// Base class for FridgeI00. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AFridgeI00: public AFridge {
	GENERATED_BODY()

public:
	AFridgeI00();
};
