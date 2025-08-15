// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "Blueprint/UserWidget.h"

#include "InventoryTypes.h"

#include "InventoryUI.generated.h"

class UInventoryItemUI;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUIDone);

// Base class for the inventory ui
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryUI: public UUserWidget {
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void Show();

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void Hide();
	
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetSelected(const FName& Name);
	
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetItemCold(const FName& Name);

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetItemMod(const FName& Name, int32 Diff, const FItem& Item);

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction, DeprecatedFunction)) // move to LInventory and use the new Items
	void SetItemUsed(const FName& Name);

	UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction)) // move to LInventory and use the new Items
	UInventoryItemUI* GetItem(const FName& Name);

	// variable that indicates when the Inventory is ready to progress.
	// false when animating.
	UPROPERTY(BlueprintReadWrite)
	bool IsReady = false;
	
	// the manager will handle this.
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Transient)
	FInventoryUIDone OnDone;

protected:
	UFUNCTION(BlueprintCallable)
	void PostHide();

	UFUNCTION(BlueprintCallable)
	void PreShow();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Transient)
	TArray<TObjectPtr<UInventoryItemUI>> WItems;
};
