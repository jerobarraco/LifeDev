// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"

#include "Templates/SharedPointer.h"

#include "Eval.generated.h"

class FMathExpEvaluator;

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(double, FJEVGetVar, const FName&, Name);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FJEVSetVar, const FString&, Name, const double, Val);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FJEVSetVarId, const double, Id, const double, Val);

// Subsystem that evaluates math expressions
// usage:
// e.g. {Var} returns the variable. {#MyId} returns the numeric value of the FName MyId.
// e.g. {V.Story.Step.Cur}={#C3S0}
//		you need to implement the "#" prefix on GetVar.
//		if you want to return a string. please use the variables with some way to detect the conversion.
//		then on getVar convert to FName and then .toUnstableInt. and memcopy to a double, or memcopy to a double directly. this might not work
// for boolean stuff anything > 0 is true. else is false.
// for some operators 0 and 1 will be returned. for others the actual value of the variable.
// Operators: +,-,/,*,% (modulo)
// sqrt, abs, sat (clamps between 0 and 1), ceil, floor, trunc (truncate), frac (fraction part), round (rounds half from zero)
// base^exp (power)
// a?b: random between a and b
// lists:
//		a,b,c,d : makes a list
//		pick(a,b,c,d) : random from a list or NAN
//		a,b,c,d[e : chooses element index e from list (a,b,c,d)
// !a: not a
// a&b a|b a$b: and or (returns the value) xor (returns 0,1)
//		a>b & c | d : ternary if: a>b?c:d (you can have any expression instead of A>B, <=0 is false, >0 is true) 
// a<b a>b a=b: < > = (returns 1: true, 0: false)
// {XXX} obtains the (double) value for a variable named "XXX" is up to the client to bind to GetVar and provide that value
//		{#XXX} (suggested) return the double value for an FName, or pointer to a variable. (See FName.ToUnstableInt)
//		This is good when trying to compare fname values. like {YYYY}={#Variable}. YYYY represents an FName returned as an unstable int.
//		When doing this. use reintepret_cast<double> instead of simply converting from int, or you'll lose precision.
// "XXX" a string variable name. Used ONLY for setting a variable. incompatible with the rest.
// ":" (without quotes). sets a value, and returns it.
//		Can be used with the numeric representation of a variable from GetVar.
//			This requires to bind to SetVarId. returns the value being set.
//			E.g.: {#XXXX}:3
//		Can be used with a string like: "XXX":3
//			This requires to bind to SetVar. Will return the value being set.
//			E.g.: "XXX":({YYY}*2)
//			E.g.: !("XXX":({ZZZ}+2))
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
	UPROPERTY(BlueprintReadWrite, Category=EvalMath)
	FJEVSetVarId OnSetVarId;

private:
	UFUNCTION()
	double GetVar(const FName Name) {
		const bool IsBound = OnGetVar.IsBound();
		UE_CLOG(!IsBound, LogTemp, Warning, TEXT("%hs, OnGetVar not bound!"), __func__);
		// just forward
		return LIKELY(IsBound) ? OnGetVar.Execute(Name) : 0; // this is more optimal. log is removed on shipping
	}

	UFUNCTION()
	void SetVar(const FString& Name, const double Val) { // just forward
		const bool IsBound = OnSetVar.IsBound(); 
		UE_CLOG(!IsBound, LogTemp, Warning, TEXT("%hs, OnSetVar not bound!"), __func__);
		if (LIKELY(IsBound)) OnSetVar.Execute(Name, Val);
	}
	
	UFUNCTION()
	void SetVarId(const double NameID, const double Val) const { // just forward
		const bool IsBound = OnSetVarId.IsBound(); 
		UE_CLOG(!IsBound, LogTemp, Warning, TEXT("%hs, OnSetVarId not bound!"), __func__);
		if (LIKELY(IsBound)) OnSetVarId.Execute(NameID, Val);
	}

	TSharedPtr<FMathExpEvaluator, ESPMode::NotThreadSafe> Evaluator = nullptr;
};
