// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Puzzle.h"

#include "PuzzleI00.generated.h"

// Chap 02 Puzzle Instance 00 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI00: public APuzzle {
	GENERATED_BODY()

public:
	APuzzleI00();

protected:
	virtual void Done_Implementation(bool IsOk) override;
	
};
