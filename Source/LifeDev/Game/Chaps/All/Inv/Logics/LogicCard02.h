// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Inventory/Logics/LogicCard.h"

#include "LogicCard02.generated.h"

// item logic for card 02 (yin)
UCLASS(Blueprintable, BlueprintType, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ULogicCard02: public ULogicCard {
	GENERATED_BODY()

public:
	ULogicCard02();
	virtual void Use_Implementation() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config)
	float ModMin = -.05;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config)
	float ModMax = -.1;
};
