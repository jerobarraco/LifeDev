// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "MuseModule.h"

IMPLEMENT_MODULE(FMuseModule, Muse)
void FMuseModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FMuseModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
