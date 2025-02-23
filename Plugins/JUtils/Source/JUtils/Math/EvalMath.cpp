// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "EvalMath.h"

#include "MathExpEvaluator.h"

DEFINE_LOG_CATEGORY_STATIC(LogEvalMath, Log, Log);

UEvalMath::UEvalMath():Super() {}

void UEvalMath::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Evaluator = MakeShared<FMathExpEvaluator, ESPMode::NotThreadSafe>();
	if (UNLIKELY(!Evaluator.IsValid())) return; // TODO warning
	Evaluator->OnGetVar.BindUObject(this, &UEvalMath::GetVar);
}

void UEvalMath::Deinitialize() {
	if (LIKELY(Evaluator.IsValid()))
		Evaluator->OnGetVar.Unbind();
	Evaluator.Reset();
	Super::Deinitialize();
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

double UEvalMath::Eval(const FString& Exp, bool& Ok) const {
	Ok = false;
	if (UNLIKELY(!Evaluator.IsValid())) return 0;
	if (UNLIKELY(Exp.TrimStartAndEnd().IsEmpty())) {
		Ok = true;
		return 0;
	}

	TValueOrError<double, FExpressionError> Result = Evaluator.Get()->Evaluate(*Exp);
	if (UNLIKELY(!Result.IsValid())) {
		UE_LOG(LogEvalMath, Warning, TEXT("%hs: error=%s exp=%s"),
			__func__, *Result.GetError().Text.ToString(), *Exp);
		return 0;
	}

	UE_LOG(LogEvalMath, Log, TEXT("%hs: Ok exp=%s res=%.3f"),
			__func__, *Exp, Result.GetValue());
	Ok = true;
	return Result.GetValue();
}
