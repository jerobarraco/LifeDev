// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI00.generated.h"

// Chap 00 Puzzle Instance 00 flight/fear
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI00: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI00();

protected:
	virtual void BeginPlay() override;
};
