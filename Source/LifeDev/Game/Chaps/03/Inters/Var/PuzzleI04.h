// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI04.generated.h"

// Chap 04 Puzzle Instance 04 Piano 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI04: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI04();

protected:
	virtual void PostLoad() override;
	// virtual void BeginPlay() override;
};
