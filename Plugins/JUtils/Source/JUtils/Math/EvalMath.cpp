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

double UEvalMath::Eval(const FString& Exp) const {
	// TOOD use nan or use bool+default?
	if (UNLIKELY(!Evaluator.IsValid())) return NAN;
	if (UNLIKELY(Exp.IsEmpty())) return NAN;
	FString Eval;
	if (UNLIKELY(!SetVars(Exp, Eval))) return NAN;

	// TODO replace variables
	TValueOrError<double, FExpressionError> Result = Evaluator.Get()->Evaluate(*Exp);
	if (UNLIKELY(!Result.IsValid())) {
		UE_LOG(LogEvalMath, Warning, TEXT("%hs: error=%s"), __func__, *Result.GetError().Text.ToString());
		return NAN;
	}

	return Result.GetValue();
}

bool UEvalMath::SetVars(const FString& Exp, FString& OExp) const {
	OExp = Exp.TrimStartAndEnd();
	if (LIKELY(OExp.IsEmpty())) return true;

	// "{xx}"	start=0, end=3, len=2, sub={xx}, name =xx
	// "{}"		start=0, end=1, len=0, sub={}, name=""

	int32 PStart = 0;
	int32 PEnd = 0;
	int32 Len = 0;
	FString Sub;
	FString VarName;
	float VarVal = 0;
	while (true) {
		PStart = OExp.Find("{");
		if (PStart<0) break; // done

		// TODO turn this into a grouping on the parser
		PEnd = OExp.Find("}", ESearchCase::IgnoreCase, ESearchDir::FromStart, PStart);
		if (UNLIKELY(PEnd <= PStart)) { // this is redundant with below, but i want to have good logs.
			UE_LOG(LogEvalMath, Warning, TEXT("%hs: Erroneous expression. Missing '}'. Exp='%s'"), __func__, *OExp);
			return false;
		}

		Len = PEnd - PStart -1; // PEnd is at BEFORE the character. so it does not contain it! (hence -1)
		Sub = OExp.Mid(PStart, Len+2);
		VarName = Sub.Mid(1, Len);
		VarName.TrimStartAndEndInline(); // in case the user enters { myvarnamelol }
		if (UNLIKELY(VarName.IsEmpty())) {
			UE_LOG(LogEvalMath, Warning,
				TEXT("%hs: Erroneous expression: Variable name is empty. need something inside {}."), __func__, *OExp);
			return false;
		}

		const bool Bound = OnGetVar.IsBound();
		VarVal = LIKELY(Bound) ? OnGetVar.Execute(FName(VarName)) : 0; // NEEEDS to check for isbound or risk a crash :')
		UE_CLOG(!Bound, LogEvalMath, Warning, TEXT("%hs: OnGetFlag is not bound! All flags are going to be 0. LOL."), __func__);

		OExp.ReplaceInline(*Sub,*FString::SanitizeFloat(VarVal,0));
	}
	return true;
}
