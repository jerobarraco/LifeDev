// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Modules/ModuleInterface.h"

class INVENTORY_API FInventoryModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
