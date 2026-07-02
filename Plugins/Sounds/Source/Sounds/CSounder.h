// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "Components/AudioComponent.h"

#include "CSounder.generated.h"

// make a baseclass for sound stuff?
// use CLSounder on LifeDev
UCLASS(Blueprintable, BlueprintType, Config=Sounds, DefaultConfig, meta=(BlueprintSpawnableComponent))
class SOUNDS_API UCSounder : public UAudioComponent {
	GENERATED_BODY()

public:
	UCSounder();

	// main entry point. fades in or out with the given parameters
	// you can still call the parent functions with less functionality
	UFUNCTION(BlueprintCallable)
	void Fade(const bool In);

	UFUNCTION(BlueprintCallable)
	void SetSafeParamFloat(const FName& Name, const float V);
	
	// fade in time or 0 for none
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float TimeFadeIn = 2.0;

	// fade out time or 0 for none
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float TimeFadeOut = 2.0;

	// time when to start the sound. See TimeStartMax.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float TimeStartMin = 0;

	// if >= 0 will randomize between min and max. otherwise it will use min.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	float TimeStartMax = -1;
};
