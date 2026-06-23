// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "SetAntiAlias.h"

#include "Misc/ConfigCacheIni.h"

ENUM_RANGE_BY_COUNT(EAntiAliasingMethod, EAntiAliasingMethod::AAM_MAX); // benui told me to do it

void USetAntiAlias::Load_Implementation() {
	ClearOptions(); // could be called multiple times during runtime.
	for (const EAntiAliasingMethod& A: TEnumRange<EAntiAliasingMethod>()) {
		// UEnum::GetValueAsString() works in shipped build, UEnum::GetDisplayValueAsText() does not.
		// https://forums.unrealengine.com/t/uenum-and-getvalueasstring/23377/14?u=nande
		// AddOption(UEnum::GetValueAsString(A));
		AddOption(GetShortAntiAliasingName(A));
	}

	// this doesn't give the updated value
	// URendererSettings* const Settings = GetMutableDefault<URendererSettings>();
	// if (Settings) {
		// SetSelectedIndex(Settings->DefaultFeatureAntiAliasing);
	// }
	
	const TConsoleVariableData<int32>* const Variable = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.AntiAliasingMethod"));
	if (LIKELY(Variable)) {
		const int32 Val = FMath::Clamp<int32>(Variable->GetValueOnAnyThread(), 0, AAM_MAX);
		SetSelectedIndex(Val); // the enum maps perfectly
	}
}

void USetAntiAlias::Apply_Implementation() {
	IConsoleVariable* const Variable =
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
	if (UNLIKELY(!Variable)) return;

	const int32 Val = FMath::Clamp<int32>(GetSelectedIndex(), 0, AAM_MAX);
	UE_LOG(LogTemp, Log, TEXT("%hs antialias to %i"), __func__, Val);
	Variable->Set(Val);
	// from DrawPrimitiveDebuggerConfig
	// TODO this is not working
	GConfig->SetInt(TEXT("/Script/Engine.RendererSettings"),
		TEXT("r.AntialiasingMethod"), Val, GEngineIni);
	GConfig->Flush(false);
}
