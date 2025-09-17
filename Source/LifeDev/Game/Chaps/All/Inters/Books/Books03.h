// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Books.h"

#include "Books03.generated.h"

// base class for interactable books. only 1 book
// this is not used at the moment. maybe i should reparent to booksB
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooks03: public ABooks {
	GENERATED_BODY()

public:
	ABooks03();
};
