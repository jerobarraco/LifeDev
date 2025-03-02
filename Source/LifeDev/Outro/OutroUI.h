// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"

#include "OutroUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutroUIDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutroUIQuit);

UCLASS(Blueprintable)
class LIFEDEV_API UOutroUI : public UBaseUI {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetFlags(const TMap<FName, float>& Flags);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetPage(const int32 Page=0);
};
