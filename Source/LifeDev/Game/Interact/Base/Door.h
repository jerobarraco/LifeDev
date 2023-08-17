// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Doorish.h"

#include "Door.generated.h"

// Base class for door-like and drawer actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor: public ADoorish {
public:
	GENERATED_BODY()

	ADoor();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Frame = nullptr;
};
