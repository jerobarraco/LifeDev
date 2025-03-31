// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LPuzzle.h"

#include "Puzzle00.generated.h"

// Generic Puzzle for phones
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APuzzle00: public ALPuzzle {
	GENERATED_BODY()

public:
	APuzzle00();

protected:
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
};
