// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT


#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Core/Settings/UI/LComboStr.h"

#include "SetAntiAlias.generated.h"

// TODO fix the style

// AntiAlias setting widget
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API USetAntiAlias: public ULComboStr {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Load();
	virtual void Load_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Apply();
	virtual void Apply_Implementation();
};
