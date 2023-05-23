// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "InventoryTypes.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventoryOnMod, const FName&, Name, const FItem&, Item, int32, NewCount);

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

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Get(const FName& Name, FItem& OutItem, int32& OutCount);
	
	// system ones ////////
	UFUNCTION(BlueprintCallable)
	void Init(UDataTable* DataTable);
	
	UFUNCTION(BlueprintCallable)
	void DeInit();

	UFUNCTION(BlueprintCallable)
	TMap<FName, int32> GetItems();

	UFUNCTION(BlueprintCallable)
	void SetItems(const TMap<FName, int32>& NewItems);
	/// ~system

	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FInventoryOnMod OnMod;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, int32> Items;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UDataTable* DT = nullptr;
};
