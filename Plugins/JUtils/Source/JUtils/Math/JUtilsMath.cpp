// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "JUtilsMath.h"

#include "Math/BasicMathExpressionEvaluator.h"

float UJUtilsMath::MathEval(const FString& Expression) {
	// TODO find better name.
	const FBasicMathExpressionEvaluator Parser;

	TValueOrError<double, FExpressionError> Result = Parser.Evaluate(*Expression);
	if (UNLIKELY(!Result.IsValid())) {
		UE_LOG(LogTemp, Warning, TEXT("%hs: error=%s"), __func__, *Result.GetError().Text.ToString());
		return NAN;
	}

	return Result.GetValue();
}
