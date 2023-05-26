// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "LInventoryItemUI.generated.h"

UCLASS(Blueprintable)
class LIFEDEV_API UInventoryItemUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetItem(const FName& Name, const FItem& Item);
};
