// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "JUtils/UI/BaseUI.h"

#include "SettingsUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSettingsUIDone);

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API USettingsUI : public UBaseUI {
	GENERATED_BODY()
public:
	USettingsUI();
};
