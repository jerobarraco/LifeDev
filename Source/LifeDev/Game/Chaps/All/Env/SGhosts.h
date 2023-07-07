// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"


#include "SGhosts.generated.h"

class UCSounder;
// make a baseclass for sound stuff? 
//  class for the Ghosts 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ASGhosts : public AActor {
	GENERATED_BODY()

public:
	ASGhosts();

	UFUNCTION(BlueprintCallable)
	void SetPlaying(bool IsPlaying);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCSounder* SFX;
};
