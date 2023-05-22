// Copyright (C) 2023 - Jerónimo Barraco-Mármol
#pragma once

#include "CoreMinimal.h"

class INVENTORY_API FInventoryModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
