// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"

#include "LGUSettings.generated.h"

// user settings and savefile
UCLASS(Blueprintable, config=LGUSettings, meta=(DisplayName="LifeDevGameUserSettings"))
class LIFEDEV_API ULGUSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	ULGUSettings(const FObjectInitializer& obj);
	// UPROPERTY(Config, EditAnywhere, Category = "My Custom Settings")
	// int32 CustomInteger;
};