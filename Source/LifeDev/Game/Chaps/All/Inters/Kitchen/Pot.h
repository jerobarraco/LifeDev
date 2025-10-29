// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Pot.generated.h"

// Base class for Pot. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APot: public ALInteract {
	GENERATED_BODY()

public:
	APot();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Lid = nullptr;
};
