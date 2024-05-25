// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "LifeDev/Game/Interact/LInteract.h"

#include "Fridge.generated.h"

// Base class for Fridge. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AFridge: public ALInteract {
	GENERATED_BODY()

public:
	AFridge();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UAudioComponent* SFXHum = nullptr;
};
