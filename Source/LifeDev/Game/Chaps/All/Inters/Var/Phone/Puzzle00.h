// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "Puzzle00.generated.h"

// Generic Puzzle for phones. Buttons should be set on the outliner.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzle00: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzle00();

protected:
	virtual void BeginPlay() override;
};
