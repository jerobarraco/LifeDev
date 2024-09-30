// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "SetAntiAlias.h"

#include "Engine/RendererSettings.h"

ENUM_RANGE_BY_COUNT(EAntiAliasingMethod, EAntiAliasingMethod::AAM_MAX); // benui told me to do it

void USetAntiAlias::Reset_Implementation() {
	ClearOptions(); // could be called multiple times during runtime.
	for (const EAntiAliasingMethod& A: TEnumRange<EAntiAliasingMethod>()) {
		// UEnum::GetValueAsString() works in shipped build, UEnum::GetDisplayValueAsText() does not.
		// https://forums.unrealengine.com/t/uenum-and-getvalueasstring/23377/14?u=nande
		// AddOption(UEnum::GetValueAsString(A));
		AddOption(GetShortAntiAliasingName(A));
	}

	URendererSettings* const Settings = URendererSettings::StaticClass()->GetDefaultObject<URendererSettings>();
	if (Settings) {
		SetSelectedIndex(Settings->DefaultFeatureAntiAliasing);
	}
	
	// TConsoleVariableData<int32>* const Variable = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.AntiAliasingMethod"));
	// if (Variable) {
		// const int32 Val = FMath::Clamp<int32>(Variable->GetValueOnAnyThread(), 0, AAM_MAX);
		// SetSelectedIndex(Val); // the enum maps perfectly
	// }
}

void USetAntiAlias::Apply_Implementation() {
	IConsoleVariable* const Variable = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
	if (Variable) {
		const int32 Val = FMath::Clamp<int32>(GetSelectedIndex(), 0, AAM_MAX);
		// from DrawPrimitiveDebuggerConfig
		GConfig->SetInt(TEXT("/Script/Engine.RendererSettings"), TEXT("r.AntialiasingMethod"), Val, GEngineIni);
		GConfig->Flush(false);
		// SetSelectedIndex(Variable->GetInt()); // the enum maps perfectly
	}
}
