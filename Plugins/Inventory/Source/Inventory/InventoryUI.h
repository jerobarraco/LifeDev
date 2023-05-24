// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "InventoryUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUIDone);

// Helps define an interaction volume
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
