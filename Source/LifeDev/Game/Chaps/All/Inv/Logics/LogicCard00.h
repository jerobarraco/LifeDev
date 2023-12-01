// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Inventory/Logics/LogicCard.h"

#include "LogicCard00.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULogicCard00: public ULogicCard {
	GENERATED_BODY()

public:
	ULogicCard00();

	virtual void Use_Implementation() override;
};
