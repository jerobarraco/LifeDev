// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Story/StoryMan.h"

#include "LStoryMan.generated.h"

// Lifedev version of the Story manager
UCLASS(Blueprintable, BlueprintType, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ALStoryMan: public AStoryMan {
	// put the generic things into the baseclass
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALStoryMan* Instance(const UObject* const O);

	ALStoryMan();
};
