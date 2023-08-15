// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Engine/DataTable.h"

#include "InventoryTypes.generated.h"

class UItemLogic;

// The base structure for ITEMS
USTRUCT(Blueprintable, BlueprintType)
struct INVENTORY_API FItem: public FTableRowBase {
	GENERATED_BODY()

public:
	// Each trigger will consume one unit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Consumable = true;
	// For things that trigger by themselves (and/or with other objects (e.g. cards))
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool SelfUsable = false;
	// For items that can be used with other objects. Tentative, might get removed. (mementos are false)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Usable = true;
	// max allowed number of items, -1 is unlimited.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxCount = -1;
	// 0 == no cooldown. used for cards.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CoolDown = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Title;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> Img = nullptr;

	// the class for the item manager
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UItemLogic> LogicType = nullptr;

	// used during runtime to keep track of the count
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	int32 Count = 0;
	// the current cool down being applied. used track how much remaining cool down there is.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	int32 ActiveCoolDown = 0;
	// whether the item is temporarily blocked (used on runtime)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	bool IsBlocked = false;
	// the logic for this item. created by the inventory on creation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	UItemLogic* Logic = nullptr;
};
