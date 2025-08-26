// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "InteractModule.h"

IMPLEMENT_MODULE(FInteractModule, Interact)

void FInteractModule::StartupModule() {
	IModuleInterface::StartupModule();

	// https://forums.unrealengine.com/t/how-to-modify-property-section-in-details-panel-in-editor/611250/4?u=nande
	FModuleManager& Man = FModuleManager::Get();
	
	IModuleInterface* const IMod = Man.GetModule("PropertyEditor");
	if (!IMod) {
		UE_LOG(LogTemp, Warning, TEXT("%hs could not load PropertyEditor module"), __func__);
		return;
	}
	FPropertyEditorModule& Mod = static_cast<FPropertyEditorModule&>(*IMod);
	const TSharedRef<FPropertySection> Section = Mod.FindOrCreateSection("Interact", "Interact", NSLOCTEXT("Interact", "Interact", "Interact"));
	Section->AddCategory("Interact");
	Section->AddCategory("SetUp");
		// FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// {
		// {
			// TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Actor", "Controller", LOCTEXT("Controller", "Controller"));
			// Section->AddCategory("Controller");
		// }
	// }

	Mod.NotifyCustomizationModuleChanged();
}

void FInteractModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
