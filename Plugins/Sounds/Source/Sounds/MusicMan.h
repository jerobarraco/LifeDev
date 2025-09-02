// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "MusicMan.generated.h"

class UCSounder;

// Music manager for a level
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Sounds),
	meta=(BlueprintSpawnableComponent))
class SOUNDS_API AMusicMan: public AInfo {
	GENERATED_BODY()

public:
	AMusicMan();
	
	// fades in or out. be careful since this creates issues when issues PlayMusic
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fade(const bool In = true);
	virtual void Fade_Implementation(const bool In);

	// for debug only
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Debug", meta=(AdvancedDisplay))
	FORCEINLINE void FadeIn() {Fade(true);}
	// for debug only
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Debug", meta=(AdvancedDisplay))
	FORCEINLINE void FadeOut() {Fade(false);}

	// be careful that calling fade just before or after this, can cause timing issues
	// plays a music with(out) fadeout of the previous if any
	// if snd is invalid it will stop the previous one
	UFUNCTION(BlueprintCallable)
	void Play(USoundBase* const Snd, const bool FadeOut = true);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UCSounder* GetPlayer() const { return Player; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION() // bind
	void SetNextMusic();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCSounder> Player = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<USoundBase> NextMusic = nullptr;
};
