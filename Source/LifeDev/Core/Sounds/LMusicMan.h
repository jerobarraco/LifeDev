// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "Sounds/MusicMan.h"

#include "LMusicMan.generated.h"

enum class EFeat : uint8;
class UCAnimator;
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
	void Init();

	// play/stop the rain
	UFUNCTION(BlueprintCallable)
	void SetRain(const bool Play) const;

	// applies a fx to the music. used for the settings.
	UFUNCTION(BlueprintCallable)
	void FadeMusicFX(const bool On) const;

	// set the flashback level. will also set the environ fb.
	// this is the intensity param for the music. not the volume.
	UFUNCTION(BlueprintCallable)
	void SetFB(float V); // bind, can't const
	
	// the flashback value for the Environ. depends on feature flag and override.
	UFUNCTION(BlueprintCallable)
	void SetEnvironFB(const float V) const;

	// turns the environ constant noise on/off
	UFUNCTION(BlueprintCallable)
	void SetEnviron(const bool On) const;

	// used to force environ off on the intro map/level
	UFUNCTION(BlueprintCallable)
	void SetEnvironOverride(const bool On);

	virtual void Fade_Implementation(const bool In) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() // bind
	void FeatUpdate(const EFeat Feat, const bool bEnabled);
	UFUNCTION() // bind
	void SetStep(AStep* const Step); // can't be const AStep
	UFUNCTION() // bind
	void AnimFXUpdate(const float Progress, const float Alpha);
	UFUNCTION()
	void AnimFXEnd(); // bind

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundSubmix> MusicSubmix = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundEffectSubmixPreset> MusicFX = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundSubmix> FXSubmix = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundEffectSubmixPreset> FXFX = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SetUp|Sub")
	TObjectPtr<UCLSounder> Rain = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Setup|Sub")
	TObjectPtr<UCLSounder> Environ = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Setup|Sub")
	TObjectPtr<UCAnimator> AnimMusicFX = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Setup|Sub")
	TObjectPtr<UCAnimator> AnimFXFX = nullptr;

	bool EnvironOverride = true;
};
