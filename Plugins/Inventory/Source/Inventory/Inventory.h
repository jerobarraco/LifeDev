// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "InventoryTypes.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventoryOnMod, const FName&, Name, const FItem&, Item, int32, NewCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnSelected, const FName&, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOnUsed, const FName&, Name);

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
	bool Mod(const FName& Name, int32 Diff, int32& OutDiff);

	// returns an item from the datatable if exists
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool GetRaw(const FName& Name, FItem& OutItem) const;

	// returns an item given the key name
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Get(const FName& Name, FItem& OutItem) const;
	
	// system ones ////////
	UFUNCTION(BlueprintCallable)
	void Init(UDataTable* DataTable);
	
	UFUNCTION(BlueprintCallable)
	void DeInit();

	// returns a list of items. Warning/KIKEN/Atchung modifying the item might modify the storage. so be careful.
	UFUNCTION(BlueprintCallable)
	const TMap<FName, FItem>& GetItems() const;

	UFUNCTION(BlueprintCallable)
	void SetItems(const TMap<FName, FItem>& NewItems);
	
	UFUNCTION(BlueprintCallable)
	const FName& GetSelected();

	// returns the next key on the list. forwards says the direction, from says which key from, if not specified it will be the selected.
	UFUNCTION(BlueprintCallable)
	FName GetNextKey(bool Forward = true, FName From = FName("")) const;
	
	UFUNCTION(BlueprintCallable)
	bool SetSelected(const FName& Name);

	UFUNCTION(BlueprintCallable)
	void Use(const FName& Name);


	/// ~system

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnMod OnMod;

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnSelected OnSelected;

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnUsed OnUsed;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName Selected;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FName, FItem> Items;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UDataTable* DT = nullptr;
};
