// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InventoryTypes.h"

#include "InventoryMan.generated.h"

class UInventory;
class UInputMappingContext;
class UInputAction;
class UInventoryUI;
class UCItemView;

// base class for the inventory manager. you can extend or use as is.
UCLASS(Blueprintable)
class INVENTORY_API AInventoryMan : public AInfo {
	GENERATED_BODY()

public:
	AInventoryMan();

	// call to initialize
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Init();

	// call to deinitialize. will be called at endplay automatically.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DeInit();

	UFUNCTION(BlueprintCallable)
	void SetVisible(const bool Vis);

	UFUNCTION(BlueprintCallable)
	void Show();
	
	UFUNCTION(BlueprintCallable)
	void Hide();

	UFUNCTION(BlueprintCallable)
	void SetSelected(const FName& Name);
	UFUNCTION(BlueprintCallable)
	void SetItemMod(const FName& Name, const int32 Diff, const FItem& Item);
	UFUNCTION(BlueprintCallable)
	void SetItemCold(const FName& Name);
	UFUNCTION(BlueprintCallable)
	void SetItemUsed(const FName& Name);

	UFUNCTION(BlueprintCallable)
	void ToggleInput(const bool Enable) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SetUp)
	int32 InputPrio = 9;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 ZOrder = 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool IsShowEnabled = true;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UInventoryUI> UIClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UInputMappingContext> Ctx = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UInputAction> ActionOpen = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UInputAction> ActionSelect = nullptr;

private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void UIDone() { Hide(); }
	UFUNCTION()
	void ActOpen() { Show(); }
	UFUNCTION()
	void ActSelect(const FInputActionValue& InputActionValue);

	UPROPERTY(Transient)
	TObjectPtr<UInventory> Inventory = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UInventoryUI> UI = nullptr;

	bool IsShowing = false;
};