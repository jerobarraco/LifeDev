// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "LGameInstance.generated.h"

class ULSave;

// LifeDev GameInstance
UCLASS()
class LIFEDEV_API ULGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	ULSave * Save = nullptr;
};
