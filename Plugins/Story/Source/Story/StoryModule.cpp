// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "StoryModule.h"

IMPLEMENT_MODULE(FStoryModule, Story)
void FStoryModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FStoryModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
