// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "InventoryModule.h"

IMPLEMENT_MODULE(FInventoryModule, Inventory)

void FInventoryModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FInventoryModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
