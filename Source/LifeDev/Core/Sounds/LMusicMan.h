// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Sounds/MusicMan.h"

#include "LMusicMan.generated.h"

class AStep;
class UCSounder;

// Lifedev version of the music manager
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ALMusicMan: public AMusicMan {
	GENERATED_BODY()

public:
	ALMusicMan();

	UFUNCTION(BlueprintCallable)
	void SetRain(bool Play);
	
	UFUNCTION(BlueprintCallable)
	static void SetRainS(UWorld* W, bool Play);
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() // bind
	void SetStep(AStep* Step);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSounder* Rain = nullptr;
};
