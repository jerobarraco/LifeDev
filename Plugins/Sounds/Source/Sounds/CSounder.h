// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"


#include "CSounder.generated.h"

// make a baseclass for sound stuff? 
//  class for the rain 
UCLASS(Blueprintable, BlueprintType)
class SOUNDS_API UCSounder : public UAudioComponent {
	GENERATED_BODY()

public:
	UCSounder();

	UFUNCTION(BlueprintCallable)
	void SetPlaying(bool InPlaying);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeFadeIn = 2.0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeFadeOut = 2.0;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeStart = 0;

	// if >= 0 will randomize between start and end. otherwise it will use start
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeEnd = -1;
};
