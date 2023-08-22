// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteractSpot.h"

#include "Spot00.generated.h"

// Spot for sisters clothes
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ASpot00: public ALInteractSpot {
	GENERATED_BODY()

public:
	ASpot00();

protected:
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
};
