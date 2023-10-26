// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "JSigModule.h"

IMPLEMENT_MODULE(FJSigModule, JSig)
void FJSigModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FJSigModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
