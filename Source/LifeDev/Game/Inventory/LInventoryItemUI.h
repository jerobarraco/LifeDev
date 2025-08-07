// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "Inventory/InventoryItemUI.h"

#include "LInventoryItemUI.generated.h"

struct FItem;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULInventoryItemUI : public UInventoryItemUI {
	GENERATED_BODY()

public:
	ULInventoryItemUI();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FText GetItemCountText(const FItem& Item);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const FSlateColor& GetItemColor(const FItem& Item);

	UFUNCTION(BlueprintCallable)
	void Fade(const bool In = true);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> AFade = nullptr;
};
