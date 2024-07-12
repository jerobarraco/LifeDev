// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "Interact/Animator/CAnimator.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "Sounds/MusicMan.h"

#include "LMusicMan.generated.h"

class UCLSounder;
class AStep;

// Lifedev version of the music manager
UCLASS(Blueprintable, BlueprintType, Placeable,
	ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ALMusicMan: public AMusicMan {
	GENERATED_BODY()

public:
	ALMusicMan();

	UFUNCTION(BlueprintCallable)
	static ALMusicMan* Instance(UWorld* W);
	// TODO use UObject* Context instead and use the meta tag

	UFUNCTION(BlueprintCallable)
	static void SetRainS(UWorld* W, bool Play);

	UFUNCTION(BlueprintCallable)
	static void FadeS(UWorld* W, bool In=true);

	UFUNCTION(BlueprintCallable)
	void SetRain(bool Play);

	UFUNCTION(BlueprintCallable)
	void SetEnviron(bool On);

	UFUNCTION(BlueprintCallable)
	void SetEnvironOverride(bool On);

	// the flashback value for the Environ
	UFUNCTION(BlueprintCallable)
	void SetEnvironFB(float V);

	// the flashback value for the Environ
	UFUNCTION(BlueprintCallable)
	void FadeFX(const bool On);

	virtual void Fade_Implementation(bool In) override;
	virtual void SetIntensity_Implementation(float V) override;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	UFUNCTION() // bind
	void FeatUpdate(EFeat Feat, bool bEnabled);
	UFUNCTION() // bind
	void SetStep(AStep* Step);
	UFUNCTION() // bind
	void AnimFXUpdate(const float Progress, const float Alpha);
	UFUNCTION()
	void AnimFXEnd(); // bind

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	USoundSubmix* MusicSubmix = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	USoundEffectSubmixPreset* MusicFX = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SetUp|Sub")
	UCLSounder* Rain = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Setup|Sub")
	UCLSounder* Environ = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Setup|Sub")
	UCAnimator* AnimMusicFX = nullptr;
	
	bool EnvironOverride = true;
};
