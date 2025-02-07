// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "DiagsModule.h"

IMPLEMENT_MODULE(FDiagsModule, Diags)

void FDiagsModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FDiagsModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
