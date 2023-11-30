// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Batts.generated.h"

// Base class for Batts
// TODO create a child class of this that it's BattsI00 and remove the reward stuff from here.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABatts: public ALInteract {
	GENERATED_BODY()

public:
	ABatts();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCQuickMesh* MeshB = nullptr;
};
