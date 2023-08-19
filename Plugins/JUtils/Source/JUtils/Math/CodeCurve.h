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

	/// basics
	
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

	// TODO check and implement the ones in UnrealMathUtility

	// Unreal 
	UFUNCTION(BlueprintCallable)
	float UOut(float p);
	UFUNCTION(BlueprintCallable)
	float UIn(float p);
	UFUNCTION(BlueprintCallable)
	float UInOut(float p);
	
	/// easings.net
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
	float InBack(float p);
	UFUNCTION(BlueprintCallable)
	float OutBack(float p);
	UFUNCTION(BlueprintCallable)
	float InOutBack(float p);
	

	/// bezier and stuff

	UFUNCTION(BlueprintCallable)
	float BBackStart(float T) {
		return UE::Curves::BezierInterp(0.f, -1.23f, 1.5f, 1.0f, T);

		// static const TArray<float> Ps = {
			// 0, -1.24, 1.5, 1};
		// return Bezier(T, Ps);
	}

	UFUNCTION(BlueprintCallable)
	float DBWave(float T) {
		static const TArray<float> Px({0.000000,1.325000,1.325000,0.000000});
		static const TArray<float> Py({0.000000,-0.845715,1.210408,0.000000});
		return DualBezier(T, Py, Px);
	}
	
	// some magic that i came up with allows to create weirder bezier curves
	UFUNCTION(BlueprintCallable)
	float DualBezier(float T, const TArray<float>Py, const TArray<float> Px);
	
	// slower than BezierInterp but allows arbitrary points
	// points are equidistant.
	UFUNCTION(BlueprintCallable)
	float Bezier(float T, const TArray<float>& Ps);

	/// test
	UFUNCTION(BlueprintCallable)
	FFloatCurve GetBEaseIn();
};

// test https://math.stackexchange.com/a/27300 
// // test
// Anim->Curve = nullptr;
// UCodeCurveLib* Lib = NewObject<UCodeCurveLib>();
// Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::UCodeCurveLib::Sin);
