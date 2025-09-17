// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"
#include "LSwitch.generated.h"

// Base class for switches. Can be buttons on a panel, light switches, etc.
// This is the most generic of the switches.
// Usually will trigger other interacts.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALSwitch: public ALInteract {
	GENERATED_BODY()

public:
	ALSwitch();
};
