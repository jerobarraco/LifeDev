// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Picture00.h"

#include "Picture01.generated.h"

// Base class for a picture frame that uses the palette version.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APicture01: public APicture00 {
	GENERATED_BODY()

public:
	APicture01();
};
