// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Inventory/Logics/CardLogic.h"

#include "CardLogic00.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API UCardLogic00: public UCardLogic {
	GENERATED_BODY()

public:
	UCardLogic00();

	virtual void Use_Implementation() override;
};
