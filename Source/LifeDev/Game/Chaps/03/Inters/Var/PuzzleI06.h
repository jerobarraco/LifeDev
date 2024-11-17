// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI06.generated.h"

class AInteractAnim;

// Chap 03 Puzzle Instance 05 Picture puzzles 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI06: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI06();

protected:
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
