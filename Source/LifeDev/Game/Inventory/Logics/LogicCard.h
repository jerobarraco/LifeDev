// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LLogic.h"

#include "LogicCard.generated.h"

class UDiags;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULogicCard: public ULLogic {
	GENERATED_BODY()

public:
	ULogicCard();
};
