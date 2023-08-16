// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"

#include "LGUSettings.generated.h"

// used for user settings
UCLASS(Blueprintable, config = LGUSettings, defaultconfig, meta=(DisplayName="LifeDevGameUserSettings"))
class LIFEDEV_API ULGUSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	ULGUSettings(const FObjectInitializer& obj);
	// UPROPERTY(Config, EditAnywhere, Category = "My Custom Settings")
	// int32 CustomInteger;
};