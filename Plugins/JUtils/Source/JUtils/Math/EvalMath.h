// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CoreMinimal.h"

#include "Templates/SharedPointer.h"

#include "EvalMath.generated.h"


class FMathExpEvaluator;
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(double, FJEVGetVar, const FName, Name);

// Subsystem that evaluates math expressions
UCLASS(Blueprintable, Category="JUtils", Config=JUtils, DefaultConfig)
class JUTILS_API UEvalMath: public UWorldSubsystem {
	GENERATED_BODY()

public:

#pragma region Base
	static UEvalMath* Instance(const UObject* const O);
	UEvalMath();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	void Init() {}; // by gamemode

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=EvalMath)
	bool UseSubsystem = true;
#pragma endregion

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	double Eval(const FString& Exp, bool& Ok) const;

	// replaces variables with values.
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	bool SetVars(const FString& Exp, FString& OExp) const;
	
	// called when a variable is needed. be sure to hook to this.
	UPROPERTY(BlueprintReadWrite, Category=EvalMath)
	FJEVGetVar OnGetVar;

private:
	TSharedPtr<FMathExpEvaluator, ESPMode::NotThreadSafe> Evaluator = nullptr;
};
