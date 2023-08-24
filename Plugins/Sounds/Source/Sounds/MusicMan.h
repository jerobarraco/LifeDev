// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "Components/AudioComponent.h"

#include "MusicMan.generated.h"

class UCSounder;

// Music manager for a level
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Sounds), meta=(BlueprintSpawnableComponent))
class SOUNDS_API AMusicMan: public AActor {
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

	// this is the intensity param, not the volume.
	UFUNCTION(BlueprintCallable)
	void SetIntensity(float V);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION() // bind
	void UpdateState(EAudioComponentPlayState PlayState);
	
	UFUNCTION() // bind
	void SetNextMusic();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="SetUp|Common")
	UCSounder* Player = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	USoundBase* NextMusic = nullptr;

	// cache the intensity to reapply on music change
	float Intensity = 0;
};
