// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "InteractModule.h"

IMPLEMENT_MODULE(FInteractModule, Interact)

void FInteractModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FInteractModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
