// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "InventoryTypes.h"
#include "Inventory.generated.h"


class UDataTable;
// World subsystem to deal with Inventory
UCLASS(Blueprintable, Category="Inventory")
class INVENTORY_API UInventory : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(UDataTable* DataTable);
	
	UFUNCTION(BlueprintCallable)
	void DeInit();
	
	// TODO is a map better?
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSlot> Slots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, int32> Items;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UDataTable* DT = nullptr;
};
