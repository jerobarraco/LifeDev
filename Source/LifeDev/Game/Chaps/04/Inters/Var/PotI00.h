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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<ALInteract> Flames = nullptr;
};
