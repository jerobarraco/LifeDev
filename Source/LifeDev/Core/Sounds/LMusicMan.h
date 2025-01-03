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
	void Init() {}
	
	UFUNCTION(BlueprintCallable)
	void SetRain(const bool Play);

	// the flashback value for the Environ
	UFUNCTION(BlueprintCallable)
	void FadeFX(const bool On) const;

	// the flashback value for the Environ. depends on feature flag and override.
	UFUNCTION(BlueprintCallable)
	void SetEnvironFB(const float V);

	// turns the environ constant noise on/off
	UFUNCTION(BlueprintCallable)
	void SetEnviron(const bool On);

	// turns the ghostpool on/off. depends on feature flag and override.
	UFUNCTION(BlueprintCallable)
	void SetGhosts(const bool On);

	// kills the ghosts.
	// All: if set it will kill the ones inactive in the pool too.
	UFUNCTION(BlueprintCallable)
	void KillGhosts(const bool All=false);

	// used to force environ off on the intro map/level
	UFUNCTION(BlueprintCallable)
	void SetEnvironOverride(const bool On);

	// used to force ghost off on the intro map/level
	UFUNCTION(BlueprintCallable)
	void SetGhostOverride(const bool On);

	virtual void Fade_Implementation(const bool In) override;
	virtual void SetFB_Implementation(float V) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() // bind
	void FeatUpdate(const EFeat Feat, const bool bEnabled);
	UFUNCTION() // bind
	void SetStep(AStep* const Step);
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
	bool GhostOverride = true;
};
