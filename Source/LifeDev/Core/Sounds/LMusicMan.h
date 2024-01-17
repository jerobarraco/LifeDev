// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Core/Settings/LSysSettings.h"
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
	static void SetRainS(UWorld* W, bool Play);

	UFUNCTION(BlueprintCallable)
	static void FadeS(UWorld* W, bool In=true);

	UFUNCTION(BlueprintCallable)
	void SetRain(bool Play);

	UFUNCTION(BlueprintCallable)
	void SetEnviron(bool On);

	virtual void Fade_Implementation(bool In) override;
	virtual void SetIntensity_Implementation(float V) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() // bind
	void FeatUpdate(EFeat Feat, bool bEnabled);
	UFUNCTION() // bind
	void SetStep(AStep* Step);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSounder* Rain = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSounder* Environ = nullptr;
};
