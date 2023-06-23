// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "DoorLike.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "DoorBase.generated.h"

// Base class for door-like and drawer actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoorBase: public ADoorLike {
public:
	GENERATED_BODY()

	ADoorBase();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Frame = nullptr;
};
