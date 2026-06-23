// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "DiagsModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FDiagsModule, Diags)

void FDiagsModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FDiagsModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
