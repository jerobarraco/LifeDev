// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only


#pragma once
#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"

#include "SetAntiAlias.generated.h"

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
