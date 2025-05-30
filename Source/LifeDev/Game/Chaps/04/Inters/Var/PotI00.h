// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Kitchen/Pot.h"

#include "PotI00.generated.h"

// Chap04 Pot00 instance
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APotI00: public APot {
	GENERATED_BODY()

public:
	APotI00();

protected:
	virtual void DoTrigger_Implementation() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<USoundBase> SNDDrops = nullptr;

	// keeps track of the interaction step. sucks, but not as much as
	// the other things i came up with, which are : none.
	// uint8 Step = 0;
	uint8 Foods = 0;
};

