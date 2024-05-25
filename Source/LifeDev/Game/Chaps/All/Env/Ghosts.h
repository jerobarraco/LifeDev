// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Ghosts.generated.h"

class UCLSounder;
class UNiagaraComponent;

// make a baseclass for sound stuff? 
//  class for the Ghosts 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AGhosts : public AActor {
	GENERATED_BODY()

public:
	AGhosts();

	UFUNCTION(BlueprintCallable)
	void SetPlaying(bool IsPlaying);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCLSounder* SFX = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UNiagaraComponent* Parts = nullptr;
};
