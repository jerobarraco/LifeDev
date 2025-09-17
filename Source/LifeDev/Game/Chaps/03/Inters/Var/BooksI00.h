// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Books.h"

#include "BooksI00.generated.h"

//ch03 book that gives the poem
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooksI00: public ABooks { // TODO reparent
	GENERATED_BODY()

public:
	ABooksI00();
};
