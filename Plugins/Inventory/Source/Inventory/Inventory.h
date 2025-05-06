// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "InventoryTypes.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventoryOnMod, const FName&, Name, int32, Diff, const FItem&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnSelected, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnUsed, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnItemCold, const FName&, Name);

DECLARE_LOG_CATEGORY_CLASS(LogInventory, Log, Log);

class UDataTable;

// World subsystem to deal with Inventory
UCLASS(Blueprintable, BlueprintType, Category="Inventory", Config=Inventory, DefaultConfig)
class INVENTORY_API UInventory : public UWorldSubsystem {
	GENERATED_BODY()

public:
	static UInventory* Instance(const UObject* const O);

#pragma region Regular
	// Used for Add, Rem, and Use. Returns false if not found.
	// Note: It does check for maxCount but will return true even when capped.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Mod(const FName& Name, const int32 Diff=1);
	// ensure the item is owned, with a quantity of 1 at least.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Ensure(const FName& Name);
	// removes an item by modding it with negative quantity.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Rem(const FName& Name);
	// removes all items. but does not trigger any onMod. used for savegame. be careful.
	UFUNCTION(BlueprintCallable, Category="Inventory", meta=(AdvancedDisplay))
	bool Clear(const int32 NumReserve = 0);
#pragma endregion
#pragma region Item
	// "uses" an item (marks as used). consumes it if it's a consumable. returns success.
	// it won't trigger the manager/item logic, you need to do it manually.
	// returns whether it was successfully used.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Use(const FName& Name);
	// un/marks an item as locked. (can't be used when locked).
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool SetLocked(const FName& Name, const bool NewBlocked);
	// force item cooldown. Does not immediately triggers OnCold. but instead it will trigger when the scheduled timer 
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool SetCool(const FName& Name);
#pragma endregion

#pragma region Gets
	// returns true if this item exists on the current inventory
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Has(const FName& Name) const;
	// returns true if the item is usable (implies cold)
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Inventory")
	bool IsUsable(const FItem& Item) const;
	// returns true if the item is cold (not waiting for cooldown) (regardless of whether it uses cooldown)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool IsCold(const FItem& Item);
	// returns how many of an item there are.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 Count(const FName& Name) const;
	// returns an item given the key name (in the loaded inventory, not the datatable)
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Get(const FName& Name, FItem& OutItem) const;
	// returns an item from the datatable if exists. Use this only if you know what you do.
	UFUNCTION(BlueprintCallable, Category="Inventory", meta=(AdvancedDisplay))
	bool GetRaw(const FName& Name, FItem& OutItem) const;
	// returns a list of items. Warning/KIKEN/Atchung modifying the item might modify the storage. so be careful.
	UFUNCTION(BlueprintCallable, Category="Inventory", meta=(AdvancedDisplay))
	const TMap<FName, FItem>& GetAll() const;
#pragma endregion

	// overrides the current items
	UFUNCTION(BlueprintCallable, Category="Inventory", meta=(AdvancedDisplay))
	void SetItems(const TMap<FName, FItem>& NewItems);

#pragma region selected
	// returns the selected item name
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	FORCEINLINE FName GetSelected() const { return Selected; }
	// returns a copy of the selected item
	UFUNCTION(BlueprintCallable, Category="Inventory") // not pure because it creates a copy
	bool GetSelectedItem(FItem& Item) const;
	// sets an item as selected
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool SetSelected(const FName& Name);
#pragma endregion

#pragma region system
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void Init(UDataTable* const DataTable);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void DeInit();
	
	// returns the next key on the list. forwards says the direction,
	// from says which key from, if not specified it will from be the selected.
	// if less than 2 items exists it will return none
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FName GetNextKey(const bool Forward = true, FName From = NAME_None) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory", Config)
	bool UseSndAutoLoad = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory", Config)
	bool UseSndAsyncLoad = true;
	// seconds between each cool down timer tick.
	// does not affect the duration, but affects the performance and granularity.
	// this should be lower than your lowest cooldown.
	// This is like a tick interval. Items are checked for cool down every CoolTimerRate-seconds.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Inventory", Config)
	float CoolTimerRate = 1;
#pragma endregion

#pragma region Delegates
	// Triggered when an item is modified
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnMod OnMod;

	// Triggered on selection change
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnSelected OnSelected;

	// Triggered when an item is used
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnUsed OnUsed;

	// Triggered when an item becomes cold
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnItemCold OnCold;
#pragma endregion

protected:
	void SetCoolTimerEnabled(const bool Enable);
	void CoolTimerTick();

	// Creates a new instance of the item and adds it to the inventory
	// returns the item or null if it doesn't exist.
	// utility function, can't be ufunction since it can't return a pointer.
	FItem* AddNew(const FName& Name);

	UFUNCTION(BlueprintCallable)
	FItem& GetRef(const FName& Name, bool& OutFound);

	UFUNCTION(BlueprintCallable)
	const FItem& GetRefC(const FName& Name, bool& OutFound) const;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FName Selected = NAME_None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient)
	TMap<FName, FItem> Items;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient)
	TObjectPtr<UDataTable> DT = nullptr;

	FTimerHandle CoolTimer;
};
