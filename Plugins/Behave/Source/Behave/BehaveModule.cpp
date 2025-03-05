// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BehaveModule.h"

IMPLEMENT_MODULE(FBehaveModule, Behave)
void FBehaveModule::StartupModule() {
	IModuleInterface::StartupModule();
}

void FBehaveModule::ShutdownModule() {
	IModuleInterface::ShutdownModule();
}
