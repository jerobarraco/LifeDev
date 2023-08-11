// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "CodeCurve.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(float, FCodeCurve, float, Progress);

UCLASS(Blueprintable)
class JUTILS_API UCodeCurveLib: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:

	// cant bind a const not static : lame :(
	
	UFUNCTION(BlueprintCallable)
	float LinCycle(float p);

	UFUNCTION(BlueprintCallable)
	float Rand(float p);
	UFUNCTION(BlueprintCallable)
	float Sin(float p);
	UFUNCTION(BlueprintCallable)
	float Cos(float p);

	UFUNCTION(BlueprintCallable)
	float SinFull(float p);
	UFUNCTION(BlueprintCallable)
	float CosFull(float p);

	UFUNCTION(BlueprintCallable)
	FFloatCurve GetBEaseIn();

	UFUNCTION(BlueprintCallable)
	float BBackStart(float T) {
		static const TArray<float> Ps = {
			0, -0.3, 0.26, 1};
		return Bezier(T, Ps);
	}

	// slower than BezierInterp but allows arbitrary points
	UFUNCTION(BlueprintCallable)
	float Bezier(float T, const TArray<float>& Ps);
};

// // test
// Anim->Curve = nullptr;
// UCodeCurveLib* Lib = NewObject<UCodeCurveLib>();
// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::UCodeCurveLib::Sin);
