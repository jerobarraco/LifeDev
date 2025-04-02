// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LSwitch04.h"

#include "LSwitch05.generated.h"

// Base for toilet button 2. inherits from LSwitch04 (button 1)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALSwitch05: public ALSwitch04 {
	GENERATED_BODY()

public:
	ALSwitch05();
};
