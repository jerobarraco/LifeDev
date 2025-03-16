// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

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
	static FORCEINLINE_DEBUGGABLE double RemapNorm(const double X, const double Min, const double Max) {
		// https://guillermoalgora.com/normalizing-remapping-values.html
		// x normalized = (x - min) / (max - min)
		return (X-Min) / (Max-Min);
	}

	template<class T>
	static constexpr FORCEINLINE_DEBUGGABLE T RemapNormT(const T& X, const T& Min, const T& Max) {
		return (X-Min) / (Max-Min);
	} 
};
