// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Inventory/Logics/LogicCard.h"

#include "LogicCard04.generated.h"

// item logic for card 04 (acceptance)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULogicCard04: public ULogicCard {
	GENERATED_BODY()

public:
	ULogicCard04();
	virtual void Use_Implementation() override;
};
