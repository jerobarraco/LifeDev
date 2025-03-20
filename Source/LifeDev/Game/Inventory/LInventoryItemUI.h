// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "LInventoryItemUI.generated.h"

struct FItem;

UCLASS(Blueprintable)
class LIFEDEV_API UInventoryItemUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetItem(const FName& Name, const FItem& Item);
	
	UFUNCTION(BlueprintCallable)
	static FText GetItemCountText(const FItem& Item);

	UFUNCTION(BlueprintCallable)
	FSlateColor GetItemColor(const FItem& Item);
};
