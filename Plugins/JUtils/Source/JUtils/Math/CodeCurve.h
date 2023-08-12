// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "Curves/CurveEvaluation.h"
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
	float OutBounce(float p);
	UFUNCTION(BlueprintCallable)
	float InBounce(float p);
	UFUNCTION(BlueprintCallable)
	float InOutBounce(float p);

	UFUNCTION(BlueprintCallable)
	float InOutCubic(float p);
	UFUNCTION(BlueprintCallable)
	float InCubic(float p);
	UFUNCTION(BlueprintCallable)
	float OutCubic(float p);
	

	
	UFUNCTION(BlueprintCallable)
	FFloatCurve GetBEaseIn();

	UFUNCTION(BlueprintCallable)
	float BBackStart(float T) {
		return UE::Curves::BezierInterp(0.f, -1.23f, 1.5f, 1.0f, T);

		// static const TArray<float> Ps = {
			// 0, -1.24, 1.5, 1};
		// return Bezier(T, Ps);
	}

	// slower than BezierInterp but allows arbitrary points
	// problem is points are equidistant.
	UFUNCTION(BlueprintCallable)
	float Bezier(float T, const TArray<float>& Ps);
};

// test https://math.stackexchange.com/a/27300 
// // test
// Anim->Curve = nullptr;
// UCodeCurveLib* Lib = NewObject<UCodeCurveLib>();
// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::UCodeCurveLib::Sin);
