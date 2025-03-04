// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Books01.h"

#include "PZBooksI01.generated.h"


// ch00 book puzzle i 01
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APZBooksI01: public ABooks01 {
	GENERATED_BODY()

public:
	APZBooksI01():Super(3, 11) {};
};
