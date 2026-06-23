// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "InventoryModule.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FInventoryModule, Inventory)

void FInventoryModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FInventoryModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
