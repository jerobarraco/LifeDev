// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "InventoryTypes.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryOnMod, const FName&, Name, const FItem&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnSelected, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnUsed, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnItemCold, const FName&, Name);

DECLARE_LOG_CATEGORY_CLASS(LogInventory, Log, Log);

class UDataTable;
// World subsystem to deal with Inventory
UCLASS(Blueprintable, Category="Inventory")
class INVENTORY_API UInventory : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	// regular ones ////////

	// Used for Add, Rem, and Use. Returns false if not found.
	// Note: It does check for maxCount but will return true even when capped.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Mod(const FName& Name, int32 Diff);

	// uses an item. removes it from the inventory if needed. returns success.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Use(const FName& Name);

	// returns an item from the datatable if exists
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool GetRaw(const FName& Name, FItem& OutItem) const;

	// returns an item given the key name
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Get(const FName& Name, FItem& OutItem) const;
	
	// returns a list of items. Warning/KIKEN/Atchung modifying the item might modify the storage. so be careful.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	const TMap<FName, FItem>& GetItems() const;

	// overrides the current items
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetItems(const TMap<FName, FItem>& NewItems);

	// returns the selected item name
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	const FName& GetSelected() const;

	// returns the selected item
	UFUNCTION(BlueprintCallable, Category="Inventory") // not pure because it creates a copy
	bool GetSelectedItem(FItem& Item) const;

	// sets an item as selected
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool SetSelected(const FName& Name);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	bool Has(const FName& Name);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	bool IsUsable(const FItem& Item) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool IsCold(const FItem& Item);

	/// system

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void Init(UDataTable* DataTable);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void DeInit();
	
	// returns the next key on the list. forwards says the direction, from says which key from, if not specified it will be the selected.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FName GetNextKey(bool Forward = true, FName From = FName("")) const;

	/// ~system

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnMod OnMod;

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnSelected OnSelected;

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnUsed OnUsed;

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnItemCold OnCold;

protected:
	void SetCoolTimerEnabled(bool Enable);
	void CoolTimerTick();

	UFUNCTION(BlueprintCallable)
	FItem& GetRef(const FName& Name, bool& OutFound);

	UFUNCTION(BlueprintCallable)
	const FItem& GetRefC(const FName& Name, bool& OutFound) const;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FName Selected;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient)
	TMap<FName, FItem> Items;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient)
	UDataTable* DT = nullptr;

	FTimerHandle CoolTimer;
};
