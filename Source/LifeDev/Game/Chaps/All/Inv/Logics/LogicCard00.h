// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Inventory/Logics/LogicCard.h"

#include "LogicCard00.generated.h"

class ARange;

// item logic for card 00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULogicCard00: public ULogicCard {
	GENERATED_BODY()

public:
	ULogicCard00();

	virtual void Use_Implementation() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ARange> RangeClass = nullptr;

protected:
	virtual void BeginPlay_Implementation() override;
	virtual void BeginDestroy() override;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ARange> Range = nullptr;
};
