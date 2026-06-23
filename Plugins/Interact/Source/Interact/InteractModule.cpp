// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "InteractModule.h"

#include "Modules/ModuleManager.h"

#include "JUtils/Editor/JUtilsEditor.h"

IMPLEMENT_MODULE(FInteractModule, Interact)

void FInteractModule::StartupModule() {
	IModuleInterface::StartupModule();

	UJUtilsEditor::AddOutlinerSection("Interact", "Interact", {
	"Interact", "SetUp", "Hint"});
}

void FInteractModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
