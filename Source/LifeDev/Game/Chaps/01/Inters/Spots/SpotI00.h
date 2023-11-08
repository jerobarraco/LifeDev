// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Spots/Spot00.h"

#include "SpotI00.generated.h"

// Spot for sisters clothes
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ASpotI00: public ASpot00 {
	GENERATED_BODY()

public:
	ASpotI00();

protected:
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
};
