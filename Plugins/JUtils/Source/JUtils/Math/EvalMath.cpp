// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "EvalMath.h"

#include "MathExpEvaluator.h"

DEFINE_LOG_CATEGORY_STATIC(LogEvalMath, Log, Log);

UEvalMath::UEvalMath():Super() {}

void UEvalMath::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Evaluator = MakeShared<FMathExpEvaluator, ESPMode::NotThreadSafe>();
}

void UEvalMath::Deinitialize() {
	Super::Deinitialize();
	Evaluator.Reset();
}

UEvalMath* UEvalMath::Instance(const UObject*const  O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!W)) return nullptr;

	UEvalMath* const EvalMathMat = W->GetSubsystem<UEvalMath>();
	return LIKELY(IsValid(EvalMathMat)) ? EvalMathMat : nullptr;
}

bool UEvalMath::ShouldCreateSubsystem(UObject* const Outer) const {
	if (!FSlateApplication::IsInitialized()) return false; // this requires the Slate dependency on Bulid.cs

	UE_LOG(LogEvalMath, Log, TEXT("%hs is=%i."),
		__func__, UseSubsystem);

	if (!UseSubsystem) {
		UE_LOG(LogEvalMath, Log, TEXT("%hs is false. The world subsystem will not be created."
			"Can be changed on the config file Interact.ini"), __func__);
		return false;
	}

	return Super::ShouldCreateSubsystem(Outer);
}

bool UEvalMath::DoesSupportWorldType(const EWorldType::Type WorldType) const {
	// i think it works on editor
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE || WorldType == EWorldType::Editor;
}
