// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"

#include "SetAntiAlias.generated.h"

// AntiAlias setting widget
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API USetAntiAlias: public UComboBoxString {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reset();
	virtual void Reset_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Apply();
	virtual void Apply_Implementation();
};
