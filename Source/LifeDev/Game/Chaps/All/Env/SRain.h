// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"


#include "SRain.generated.h"

// make a baseclass for sound stuff? 
//  class for the rain 
UCLASS(Blueprintable, BlueprintType)
class ASRain : public AActor {
	GENERATED_BODY()

public:
	ASRain();

	UFUNCTION(BlueprintCallable)
	void Play();

	UFUNCTION(BlueprintCallable)
	void Stop();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float FadeIn = .5;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float FadeOut = 2.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAudioComponent* SFX;
};
