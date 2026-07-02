// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "Blueprint/UserWidget.h"

#include "InventoryItemUI.generated.h"

struct FItem;

UCLASS(Blueprintable, BlueprintType)
class INVENTORY_API UInventoryItemUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetItem(const FName& IName, const FItem& Item);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetSelected(const bool Selected);
	
	// Called when the item is used
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Use();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name = NAME_None;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AnimUseSpeed = 1;

	UPROPERTY(BlueprintReadWrite, Transient)
	bool IsSelected = false;

	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> AUse = nullptr; // has te be transient or it won't compile.
};
