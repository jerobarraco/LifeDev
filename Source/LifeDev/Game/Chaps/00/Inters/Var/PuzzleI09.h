// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Phone/Puzzle00.h"

#include "PuzzleI09.generated.h"

class AStep;

// Room03 instance of the phone.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI09: public APuzzle00 {
	GENERATED_BODY()

public:
	APuzzleI09();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void StartStep(AStep* const Step);
};
