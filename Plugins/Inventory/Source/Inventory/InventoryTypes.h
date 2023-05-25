// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Engine/DataTable.h"

#include "InventoryTypes.generated.h"


// The base structure for ITEMS
USTRUCT(Blueprintable, BlueprintType)
struct INVENTORY_API FItem: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Consumable = true;
	// an item that can be used, or only held. Tentative, might get removed
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Usable = true;
	// max allowed number of items, -1 is unlimited.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxCount = -1;
	// 0 == no cooldown. used for cards.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Cooldown = 0;
	
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

	// used during runtime to keep track of the cooldown
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	int32 LastUsed = 0;
};
