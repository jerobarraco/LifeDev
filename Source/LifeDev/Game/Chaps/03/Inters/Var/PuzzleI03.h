// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI03.generated.h"

// Chap 03 Puzzle Instance 03 Fawn/(Emotion?)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI03: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI03();

protected:
	virtual void BeginPlay() override;
};
