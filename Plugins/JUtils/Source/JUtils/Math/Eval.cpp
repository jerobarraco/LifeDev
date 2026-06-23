// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "Eval.h"

#include "Engine/World.h"
#include "MathExpEvaluator.h"
#include "Framework/Application/SlateApplication.h"

DEFINE_LOG_CATEGORY_STATIC(LogEval, Log, All);

UEval::UEval():Super() {}

void UEval::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Evaluator = MakeShared<FMathExpEvaluator, ESPMode::NotThreadSafe>();
	if (UNLIKELY(!Evaluator.IsValid())) {
		UE_LOG(LogEval, Warning, TEXT("%hs Could not create evaluator! This won't work."), __func__);
		return;
	}

	Evaluator->OnGetVar.BindUObject(this, &UEval::GetVar);
	Evaluator->OnSetVar.BindUObject(this, &UEval::SetVar);
	Evaluator->OnSetVarId.BindUObject(this, &UEval::SetVarId);
}

void UEval::Deinitialize() {
	if (LIKELY(Evaluator.IsValid())) {
		Evaluator->OnGetVar.Unbind();
		Evaluator->OnSetVar.Unbind();
		Evaluator->OnSetVarId.Unbind();
	}

	Evaluator.Reset();
	OnGetVar.Clear();
	Super::Deinitialize();
}

UEval* UEval::Instance(const UObject*const  O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!W)) return nullptr;

	UEval* const EvalMathMat = W->GetSubsystem<UEval>();
	return LIKELY(IsValid(EvalMathMat)) ? EvalMathMat : nullptr;
}

bool UEval::ShouldCreateSubsystem(UObject* const Outer) const {
	if (!FSlateApplication::IsInitialized()) return false; // this requires the Slate dependency on Bulid.cs

	UE_LOG(LogEval, Log, TEXT("%hs is=%i."),
		__func__, UseSubsystem);

	if (!UseSubsystem) {
		UE_LOG(LogEval, Log, TEXT("%hs is false. The world subsystem will not be created."
			"Can be changed on the config file Interact.ini"), __func__);
		return false;
	}

	return Super::ShouldCreateSubsystem(Outer);
}

bool UEval::DoesSupportWorldType(const EWorldType::Type WorldType) const {
	// i think it works on editor
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE || WorldType == EWorldType::Editor;
}

bool UEval::Eval(const FString& Exp, double& Res, const bool IsEmptySuccess) const {
	if (UNLIKELY(Exp.TrimStartAndEnd().IsEmpty())) {
		Res = IsEmptySuccess ? 1 : 0;
		return true;
	}

	Res = -1.0;
	if (UNLIKELY(!Evaluator.IsValid())) return false;

	TValueOrError<double, FExpressionError> Result = Evaluator.Get()->Evaluate(*Exp);
	if (UNLIKELY(!Result.IsValid())) {
		UE_LOG(LogEval, Warning, TEXT("%hs: error=%s exp=%s"),
			__func__, *Result.GetError().Text.ToString(), *Exp);
		return false;
	}

	Res = Result.GetValue();
	UE_LOG(LogEval, Log, TEXT("%hs: Ok exp=%s res=%.3f"),
		__func__, *Exp, Res);
	return true;
}
