// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "PuzzleI07.generated.h"

// Chap 00 Puzzle Instance 00 flight/fear
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzleI07: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzleI07();

protected:
	virtual void PostLoad() override;
	// virtual void BeginPlay() override;
};
