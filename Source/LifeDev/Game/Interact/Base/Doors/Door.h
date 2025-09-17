// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Doorish.h"

#include "Door.generated.h"

// Base class for doors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor: public ADoorish {
	GENERATED_BODY()

public:
	ADoor();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Frame = nullptr;
};
