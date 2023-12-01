// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Components/InputKeySelector.h"

#include "LInputSelector.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULInputSelector : public UInputKeySelector {
	GENERATED_BODY()
	
public:
	ULInputSelector();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InputName; // the one used in the player mappable input key stuff very long name persistence that does not work and will crash your game_experimental
};
