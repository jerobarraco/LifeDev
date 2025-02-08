// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Story/StoryManager.h"

#include "LStoryMan.generated.h"


// TODO subclass to ALStoryMan and use a new widget ui
// TODo use a custom widget, copy the one from the story plugin
// todo think about it: should i show the saving text there?

// Lifedev version of the Story manager
UCLASS(Blueprintable, BlueprintType, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ALStoryMan: public AStoryManager {
	// put the generic things into the baseclass
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALStoryMan* Instance(const UObject* const O);
};
