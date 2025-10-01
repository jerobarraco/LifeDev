// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI05.generated.h"

// Chap 04 Puzzle Instance 05 Acceptance/??
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI05: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI05();

protected:
	virtual void BeginPlay() override;
};
