// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CoreMinimal.h"

#include "Templates/SharedPointer.h"

#include "Eval.generated.h"


class FMathExpEvaluator;
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(double, FJEVGetVar, const FName, Name);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FJEVSetVar, const uint64, NameID, const double, Val);

// Subsystem that evaluates math expressions
// if you want to compare strings. please use the variables with some way to detect the conversion.
// then on getVar convert to FName and then .toUnstableInt. and use int value instead.
// e.g. {Var} returns the variable. {#MyId} returns the numeric value of the FName MyId.
// e.g. {V.Story.Step.Cur}={#C3S0}
// for boolean stuff anything > 0 is true. else is false.
// for some operators 0 and 1 will be returned. for others the actual value of the variable.
// Operators: +,-,/,*,%
// sqrt, abs, sat (clamps between 0 and 1)
// a?b: random between a and b
// !a: not a 
// a&b a|b a$b: and or (returns the value) xor (returns 0,1) 
// a<b a>b a=b: < > =
UCLASS(Blueprintable, Category="JUtils", Config=JUtils, DefaultConfig)
class JUTILS_API UEval: public UWorldSubsystem {
	GENERATED_BODY()

public:

#pragma region Base
	static UEval* Instance(const UObject* const O);
	UEval();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	void Init() {}; // by gamemode

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=EvalMath)
	bool UseSubsystem = true;
#pragma endregion

	// empty evaluates to (ok, 0)
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	bool Eval(const FString& Exp, double& Res) const;

	// called when a variable is needed. be sure to hook to this.
	UPROPERTY(BlueprintReadWrite, Category=EvalMath)
	FJEVGetVar OnGetVar;
	// called when a variable is needed. be sure to hook to this.
	UPROPERTY(BlueprintReadWrite, Category=EvalMath)
	FJEVSetVar OnSetVar;

private:
	UFUNCTION()
	double GetVar(const FName Name) { // just forward
		if (LIKELY(OnGetVar.IsBound()))
			return OnGetVar.Execute(Name);

		UE_LOG(LogTemp, Warning, TEXT("%hs, OnGetVar not bound!"), __func__);
		return 0;
	}

	UFUNCTION()
	void SetVar(const uint64 NameID, const double Val) { // just forward
		if (LIKELY(OnSetVar.IsBound())) {
			OnSetVar.Execute(NameID, Val);
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("%hs, OnSetVar not bound!"), __func__);
	}

	TSharedPtr<FMathExpEvaluator, ESPMode::NotThreadSafe> Evaluator = nullptr;
};
