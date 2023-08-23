// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Components/AudioComponent.h"

#include "MusicMan.generated.h"

class UCSounder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCMusicManRawOnPlay);

// Plays a sound at random intervals, at a random distance, on a random point in a cone facing on the back of the owner actor.
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API AMusicMan: public AActor {
	GENERATED_BODY()

public:
	AMusicMan();
	inline static bool Enabled = true;

	UFUNCTION(BlueprintCallable)
	void Fade(bool In);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Debug")
	FORCEINLINE void FadeIn() {Fade(true);}
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Debug")
	FORCEINLINE void FadeOut() { Fade(false);}

	UFUNCTION(BlueprintCallable)
	void PlayMusic(USoundBase* Snd, bool FadeOut = true);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void UpdateState(EAudioComponentPlayState PlayState);
	UFUNCTION(BlueprintCallable)
	void SetIntensity(float V);

	UFUNCTION() // bind
	void SetNextMusic();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	UCSounder* Player = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	USoundBase* NextMusic = nullptr;
};
