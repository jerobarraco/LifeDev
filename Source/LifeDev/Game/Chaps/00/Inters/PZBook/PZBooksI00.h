// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Books01.h"

#include "PZBooksI00.generated.h"


// base class for interactable books. used for puzzles.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APZBooksI00: public ABooks01 {
	GENERATED_BODY()

public:
	APZBooksI00();
};
