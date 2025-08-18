// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT


#pragma once
#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"

#include "SetAntiAlias.generated.h"

// TODO move to lifedev, and inherit from lcombostr, and fix the style
// AntiAlias setting widget
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API USetAntiAlias: public UComboBoxString {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Load();
	virtual void Load_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Apply();
	virtual void Apply_Implementation();
};
