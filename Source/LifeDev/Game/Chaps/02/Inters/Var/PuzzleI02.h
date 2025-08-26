// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI02.generated.h"

// Chap 02 Puzzle Instance 02 Freeze/Sad 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI02: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI02();

protected:
	// virtual void PostLoad() override; // avoid this. it only runs on editor load.
	// it makes it so that on begin play some stuff is not applied, and then there are weird bugs.
	virtual void BeginPlay() override;
};
