// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "JSigModule.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FJSigModule, JSig)
void FJSigModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FJSigModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
