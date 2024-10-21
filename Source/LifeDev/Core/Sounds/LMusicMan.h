// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "Interact/Animator/CAnimator.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "Sounds/MusicMan.h"

#include "LMusicMan.generated.h"

class AGhostPool;
class UCLSounder;
class AStep;

// Lifedev version of the music manager
UCLASS(Blueprintable, BlueprintType, Placeable,
	ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ALMusicMan: public AMusicMan {
	GENERATED_BODY()

public:
	ALMusicMan();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="W"))
	static ALMusicMan* Instance(const UObject* const O);

	UFUNCTION(BlueprintCallable)
	static void SetRainS(const UWorld* const W, const bool Play);

	UFUNCTION(BlueprintCallable)
	static void FadeS(const UWorld* const W, const bool In=true);

	// Called by LGGameMode
	void Init() {};
	
	UFUNCTION(BlueprintCallable)
	void SetRain(const bool Play);

	UFUNCTION(BlueprintCallable)
	void SetEnviron(const bool On);

	UFUNCTION(BlueprintCallable)
	void SetEnvironOverride(const bool On);

	// the flashback value for the Environ
	UFUNCTION(BlueprintCallable)
	void SetEnvironFB(const float V);

	// the flashback value for the Environ
	UFUNCTION(BlueprintCallable)
	void FadeFX(const bool On);

	// kills the ghosts.
	// All: if set it will kill the ones inactive in the pool too.
	UFUNCTION(BlueprintCallable)
	void KillGhosts(const bool All=false);
	
	virtual void Fade_Implementation(bool In) override;
	virtual void SetIntensity_Implementation(float V) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetGhosts(bool bEnabled);
	UFUNCTION() // bind
	void FeatUpdate(EFeat Feat, bool bEnabled);
	UFUNCTION() // bind
	void SetStep(AStep* Step);
	UFUNCTION() // bind
	void AnimFXUpdate(const float Progress, const float Alpha);
	UFUNCTION()
	void AnimFXEnd(); // bind

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundSubmix> MusicSubmix = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundEffectSubmixPreset> MusicFX = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SetUp|Sub")
	TObjectPtr<UCLSounder> Rain = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Setup|Sub")
	TObjectPtr<UCLSounder> Environ = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Setup|Sub")
	TObjectPtr<UCAnimator> AnimMusicFX = nullptr;

	// doesn't belong here but the game mode is bloated. to be moved later.
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<AGhostPool> GhostPool = nullptr;

	bool EnvironOverride = true;
};
