// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "Inventory/ItemMan.h"

#include "CardMan.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API UCardMan: public UItemMan {
public:
	GENERATED_BODY()

	UCardMan();
	virtual void Look_Implementation() override {};
	virtual void Use_Implementation() override;
};
