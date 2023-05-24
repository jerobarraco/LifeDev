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
	
	UFUNCTION(BlueprintCallable)
	const FName& GetSelected();

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, int32> Items;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UDataTable* DT = nullptr;
};
