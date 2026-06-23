// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsEditor.h"

#if WITH_EDITOR
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "Kismet2/DebuggerCommands.h"
#endif

bool UJUtilsEditor::AddOutlinerSection(const FString& Class, const FString& Section,
	const TArray<FString>& Categories) {
#if !WITH_EDITOR
	return false;
#else
	// https://forums.unrealengine.com/t/how-to-modify-property-section-in-details-panel-in-editor/611250/4?u=nande
	FModuleManager& Man = FModuleManager::Get();
	IModuleInterface* const IMod = Man.GetModule("PropertyEditor");
	if (UNLIKELY(!IMod)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs could not load PropertyEditor module"), __func__);
		return false;
	}

	FPropertyEditorModule& Mod = static_cast<FPropertyEditorModule&>(*IMod);
	const TSharedRef<FPropertySection> Sect = Mod.FindOrCreateSection(*Class,
		*Section, FText::FromString(Section));
	for (const FString& C: Categories)
		Sect->AddCategory(*C);
	
	Mod.NotifyCustomizationModuleChanged();
	return true;
#endif
}

bool UJUtilsEditor::PlayInEditor() {
#if !WITH_EDITOR
	return false;
#else
	// https://forums.unrealengine.com/t/starting-pie-programmatically/1213447/2?u=nande
	const FUICommandList* const Actions = FPlayWorldCommands::GlobalPlayWorldActions.Get();
	if (UNLIKELY(!Actions)) return false;

	const FPlayWorldCommands& Commands = FPlayWorldCommands::Get();
	return Actions->ExecuteAction(Commands.PlayInViewport.ToSharedRef());
#endif
}


//
// void UJUtilsEditor::PostLoad() {
// 	Super::PostLoad();
// #if WITH_EDITOR
// 	FEditorDelegates::BeginPIE.AddUObject(this, &UJUtilsEditor::BeginPie);
// #endif
// }
//
// void UJUtilsEditor::SetOnPie(const FJUEOnPie& NOnPie) {
// 	UJUtilsEditor* const UtilsEditor = GetMutableDefault<UJUtilsEditor>();
// 	if (!UtilsEditor) {
// 		UE_LOG(LogTemp, Warning, TEXT("%hs can't get default"), __func__);
// 		return;
// 	}
// 	
// 	UtilsEditor->OnPie = NOnPie;
// 	UtilsEditor->OnPieD.Add(NOnPie);
// #if WITH_EDITOR
// 	auto L = [NOnPie](const bool IsSim) {
// 		NOnPie.ExecuteIfBound(IsSim);
// 	};
// 	FEditorDelegates::BeginPIE.AddLambda(L);
// #endif
// }
