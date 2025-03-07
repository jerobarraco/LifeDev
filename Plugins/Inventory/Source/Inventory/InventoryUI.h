// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "Blueprint/UserWidget.h"

#include "InventoryTypes.h"

#include "InventoryUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUIDone);

// Base class for the inventory ui
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryUI: public UUserWidget {
public:
	GENERATED_BODY()
	
	UFUNCTION(BlueprintNativeEvent)
	void Show();

	UFUNCTION(BlueprintNativeEvent)
	void Hide();
	
	UFUNCTION(BlueprintNativeEvent)
	void SetSelected(const FName& Name);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetItemCold(const FName& Name);

	UFUNCTION(BlueprintNativeEvent)
	void SetItemMod(const FName& Name, int32 Diff, const FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void SetItemUsed(const FName& Name);

	// variable that indicates when the Inventory is ready to progress.
	// false when animating.
	UPROPERTY(BlueprintReadWrite)
	bool IsReady = false;
	
	// the manager will handle this.
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FInventoryUIDone OnDone;
	
protected:
	UFUNCTION(BlueprintCallable)
	void PostHide();

	UFUNCTION(BlueprintCallable)
	void PreShow();
};
