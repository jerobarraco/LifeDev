// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI01.generated.h"

// Chap 01 Puzzle Instance 01 fight/anger 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI01: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI01();

protected:
	virtual void BeginPlay() override;
};
