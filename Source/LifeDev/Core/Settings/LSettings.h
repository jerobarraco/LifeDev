// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CoreMinimal.h"
#include "LSettings.generated.h"

// used for user settings
UCLASS(config = MySetting)
class LIFEDEV_API ULSettings : public UObject
{
	GENERATED_BODY()

public:
	ULSettings(const FObjectInitializer& obj);


	// UPROPERTY(Config, EditAnywhere, Category = "My Custom Settings")
	// int32 CustomInteger;
};