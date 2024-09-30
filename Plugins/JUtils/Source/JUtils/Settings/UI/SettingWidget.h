// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SettingWidget.generated.h"

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API USettingWidget: public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reset();
	virtual void Reset_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Apply();
	virtual void Apply_Implementation();
};
