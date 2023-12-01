// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LItemLogic.h"

#include "CardLogic.generated.h"

class UDiags;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API UCardLogic: public ULItemLogic {
	GENERATED_BODY()

public:
	UCardLogic();
	virtual void Look_Implementation() override {};
	virtual void Use_Implementation() override;
};
