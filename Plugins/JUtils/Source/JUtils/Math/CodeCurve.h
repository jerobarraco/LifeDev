// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "CoreMinimal.h"

#include "Curves/CurveEvaluation.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CodeCurve.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(float, FCodeCurve, const float, Progress);

// TODO consts

// 	UCodeCurveLib* const CurveLib = UCodeCurveLib::Instance();
// Anim->CodeCurve.BindDynamic(CurveLib, &UCodeCurveLib::OutBack);
UCLASS(Blueprintable)
class JUTILS_API UCodeCurveLib: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static UCodeCurveLib* Instance(); // TODO ! This ain't no working. fix.
	
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
	UFUNCTION(BlueprintCallable)
	float UStep(float p);
	UFUNCTION(BlueprintCallable)
	float USinIn(float p);
	UFUNCTION(BlueprintCallable)
	float USinOut(float p);
	UFUNCTION(BlueprintCallable)
	float USinInOut(float p);
	UFUNCTION(BlueprintCallable)
	float UCircularIn(float p);
	UFUNCTION(BlueprintCallable)
	float UCircularOut(float p);
	UFUNCTION(BlueprintCallable)
	float UCircularInOut(float p);

	/// easings.net
	UFUNCTION(BlueprintCallable)
	float InSin(const float p);
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
	float InOutQuart(const float p);
	
	UFUNCTION(BlueprintCallable)
	float InBack(float p);
	UFUNCTION(BlueprintCallable)
	float OutBack(float p);
	UFUNCTION(BlueprintCallable)
	float InOutBack(float p);

	UFUNCTION(BlueprintCallable)
	float InExpo(float p);
	UFUNCTION(BlueprintCallable)
	float OutExpo(float p);
	UFUNCTION(BlueprintCallable)
	float InOutExpo(float p);

	/// bezier and stuff
	UFUNCTION(BlueprintCallable)
	float BOutInQuad(float T);

	UFUNCTION(BlueprintCallable)
	float BBackStart(float T) {
		return UE::Curves::BezierInterp(0.f, -1.23f, 1.5f, 1.0f, T);

	}

	UFUNCTION(BlueprintCallable)
	float DBOutIn(float T) {
		static const TArray<float> Px({0.000000,1.325000,1.325000,0.000000});
		static const TArray<float> Py({0.000000,-0.845715,1.210408,0.000000});
		return DualBezier(T, Py, Px);
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
