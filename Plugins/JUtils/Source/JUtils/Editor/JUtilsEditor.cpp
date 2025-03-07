// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsEditor.h"
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
