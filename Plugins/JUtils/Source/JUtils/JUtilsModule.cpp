// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "JUtilsModule.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FJUtilsModule, JUtils)
void FJUtilsModule::StartupModule() {
	IModuleInterface::StartupModule();
}

void FJUtilsModule::ShutdownModule() {
	IModuleInterface::ShutdownModule();
}
