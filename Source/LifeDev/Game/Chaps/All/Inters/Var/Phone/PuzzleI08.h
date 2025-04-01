// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Puzzle00.h"
#include "Story/Step.h"

#include "PuzzleI08.generated.h"

// Room03 instance of the phone.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI08: public APuzzle00 {
	GENERATED_BODY()

public:
	APuzzleI08();
	UFUNCTION()
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	void SetStep(AStep* const Step);
};
