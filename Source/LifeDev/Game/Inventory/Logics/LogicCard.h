// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LLogic.h"

#include "LogicCard.generated.h"

class UDiags;

// base class for item logics for card.
// create a child class for specific cards.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULogicCard: public ULLogic {
	GENERATED_BODY()

public:
	ULogicCard();
};
