// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "Inventory/ItemLogic.h"

#include "CardLogic.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API UCardLogic: public UItemLogic {
public:
	GENERATED_BODY()

	UCardLogic();
	virtual void Look_Implementation() override {};
	virtual void Use_Implementation() override;
};
