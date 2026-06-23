// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "JUtilsMath.generated.h"

UCLASS(Blueprintable)
class JUTILS_API UJUtilsMath: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	// evaluates a math expression, with simple logic. "+-*/" "^"(exponent) "Sqrt(x)" square root.
	UFUNCTION(BlueprintCallable)
	static float MathEval(const FString& Expression);

	// remaps a value to a normalized value
	UFUNCTION(BlueprintCallable)
	static FORCEINLINE double RemapNorm(const double X, const double Min, const double Max) {
		// https://guillermoalgora.com/normalizing-remapping-values.html
		// x normalized = (x - min) / (max - min)
		return (X-Min) / (Max-Min);
	}

	template<class T>
	static constexpr FORCEINLINE T RemapNormT(const T& X, const T& Min, const T& Max) {
		return (X-Min) / (Max-Min);
	}

	// only works for A's that are likely not bigger than M. and for ints.
	static const FORCEINLINE int32 FastMod(const int32 A, const int32 M) {
		return LIKELY(A<M) ? A : A%M;
	}
	
	// only works for A's that are likely not bigger than M.
	template<class T>
	static const FORCEINLINE T FastModT(const T A, const T M) {
		return LIKELY(A<M) ? A : A%M;
	}
};
