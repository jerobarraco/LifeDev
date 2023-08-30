// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Sounds/MusicMan.h"

#include "LMusicMan.generated.h"


// Lifedev version of the music manager
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ALMusicMan: public AMusicMan {
	GENERATED_BODY()

public:
	

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
