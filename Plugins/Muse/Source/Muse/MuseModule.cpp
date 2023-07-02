// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "MuseModule.h"

IMPLEMENT_MODULE(FMuseModule, MuseModule)
void FMuseModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FMuseModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
