// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "StoryModule.h"

IMPLEMENT_MODULE(FStoryModule, StoryModule)
void FStoryModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FStoryModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
