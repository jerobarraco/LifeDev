// Copyright (C) 2023 - Jerónimo Barraco-Mármol
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
