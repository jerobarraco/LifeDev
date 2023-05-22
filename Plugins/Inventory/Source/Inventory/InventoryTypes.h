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
	FText Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> Img = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> Obj = nullptr;
};
