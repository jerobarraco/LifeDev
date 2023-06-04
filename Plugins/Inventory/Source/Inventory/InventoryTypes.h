// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Engine/DataTable.h"

#include "InventoryTypes.generated.h"


// The base structure for ITEMS
USTRUCT(Blueprintable, BlueprintType)
struct INVENTORY_API FItem: public FTableRowBase {
	GENERATED_BODY()

public:
	// Each trigger will consume one unit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Consumable = true;
	// For things that trigger by themselves (not with other objects (e.g. cards))
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool AutoTrigger = false;
	// an item that can be used, or only held. Tentative, might get removed. (mementos are false)
	// TODO change the name because it's ambiguous
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
	// the action text for this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Action;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> Img = nullptr;
	// do i need this? TSubClass or TSubPtr
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> Obj = nullptr;

	// used during runtime to keep track of the count
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	int32 Count = 0;

	// the current cool down being applied. used track how much remaining cool down there is.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	int32 ActiveCoolDown = 0;
};
