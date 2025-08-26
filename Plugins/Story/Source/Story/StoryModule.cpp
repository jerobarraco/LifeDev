// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "StoryModule.h"

#include "JUtils/Editor/JUtilsEditor.h"

IMPLEMENT_MODULE(FStoryModule, Story)

void FStoryModule::StartupModule() {
	IModuleInterface::StartupModule();
	UJUtilsEditor::AddOutlinerSection("Step", "Step", {"Step", "SetUp"});
}

void FStoryModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
