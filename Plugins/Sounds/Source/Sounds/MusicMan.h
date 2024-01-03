// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "MusicMan.generated.h"

class UCSounder;

// Music manager for a level
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Sounds), meta=(BlueprintSpawnableComponent))
class SOUNDS_API AMusicMan: public AActor {
	GENERATED_BODY()

public:
	AMusicMan();
	
	// fades in or out. be careful since this creates issues when issues alongside PlayMusic
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fade(bool In = true);
	virtual void Fade_Implementation(bool In);

	// for debug only
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Debug")
	FORCEINLINE void FadeIn() {Fade(true);}
	// for debug only
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Debug")
	FORCEINLINE void FadeOut() {Fade(false);}

	// be careful that calling fade just before or after playmusic can cause timing issues
	// plays a music with(out) fadeout of the previous if any
	// if snd is invalid it will stop the previous one
	UFUNCTION(BlueprintCallable)
	void PlayMusic(USoundBase* Snd, bool FadeOut = true);

	// this is the intensity param, not the volume.
	UFUNCTION(BlueprintCallable)
	void SetIntensity(float V);

protected:
	virtual void BeginPlay() override;

	UFUNCTION() // bind
	void AudioFinished();

	UFUNCTION() // bind
	void SetNextMusic();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SetUp|Common")
	UCSounder* Player = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	USoundBase* NextMusic = nullptr;

	// cache the intensity to reapply on music change
	float Intensity = 0;
};
