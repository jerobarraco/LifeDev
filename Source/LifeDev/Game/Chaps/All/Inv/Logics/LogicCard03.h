// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Inventory/Logics/LogicCard.h"

#include "LogicCard03.generated.h"

// item logic for card 03 (perseverance)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULogicCard03: public ULogicCard {
	GENERATED_BODY()

public:
	ULogicCard03();
	virtual void Use_Implementation() override;
};
