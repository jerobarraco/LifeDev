// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Spots/Spot00.h"

#include "SpotI02.generated.h"

class ALInteract;

// Spot for room03 (bathroom)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ASpotI02: public ASpot00 {
	GENERATED_BODY()

public:
	ASpotI02();
	
protected:
	virtual void Trigger_Implementation() override;

	// the reward card
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	ALInteract* Card = nullptr;
};
