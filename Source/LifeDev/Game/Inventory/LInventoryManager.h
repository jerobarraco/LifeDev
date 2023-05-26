// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"

#include "Inventory/InventoryManager.h"
#include "LInventoryManager.generated.h"

UCLASS(Blueprintable)
class LIFEDEV_API ALInventoryManager : public AInventoryManager {
	GENERATED_BODY()

public:
	ALInventoryManager();
};
