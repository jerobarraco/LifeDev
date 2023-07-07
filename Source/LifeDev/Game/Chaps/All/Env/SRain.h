// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"


#include "SRain.generated.h"

class UCSounder;
// make a baseclass for sound stuff? 
//  class for the rain 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ASRain : public AActor {
	GENERATED_BODY()

public:
	ASRain();

	UFUNCTION(BlueprintCallable)
	void SetPlaying(bool IsPlaying);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCSounder* SFX;
};
