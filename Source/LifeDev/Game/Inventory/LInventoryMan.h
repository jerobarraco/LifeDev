// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"

#include "Inventory/InventoryManager.h"
#include "LInventoryMan.generated.h"

UCLASS(Blueprintable)
class LIFEDEV_API ALInventoryMan : public AInventoryManager {
	GENERATED_BODY()

public:
	ALInventoryMan();
};
