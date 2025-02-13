// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SettingWidget.generated.h"

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API USettingWidget: public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Load();
	virtual void Load_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Apply();
	virtual void Apply_Implementation();
};
