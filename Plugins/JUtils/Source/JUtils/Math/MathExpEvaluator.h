// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreTypes.h"
#include "Internationalization/FastDecimalFormat.h"
#include "Misc/ExpressionParserTypes.h"
#include "Misc/Optional.h"
#include "Templates/ValueOrError.h"

// this way it doesn't need the .generated. which needs a uclass. which we don't have.
DECLARE_DELEGATE_RetVal_OneParam(double, FJEXVGetVar, const FName);
DECLARE_DELEGATE_TwoParams(FJEXVSetVarId, const double, const double);
DECLARE_DELEGATE_TwoParams(FJEXVSetVar, const FString&, const double);

struct FDecimalNumberFormattingRules;

// it HAS to be a guid. and i have NO idea how to get one.
// otherwise. if i use string constants, even though they are different. they WILL map to one of them. breaking everyting.
// i've just manually entered some random ones. based on a copy from somewhere else.
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FSaturate, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x27060734)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FAbsolute,  0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060720)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FRand, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060721)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FNot, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060722)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FAnd, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060723)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FOr, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060724)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FXor, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060756)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FGreatThan, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060766)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FLessThan, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x2106077)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FEquals, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060728)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FSet, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060729)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FItem, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060730)

DEFINE_EXPRESSION_NODE_TYPE(TArray<double>, 0x03378491, 0x42D14E27, 0x8E95AD27, 0x74567867)

// A basic math expression evaluator. with variables. and logic. and not gambling.
class JUTILS_API FMathExpEvaluator {
public:
	// Constructor that sets up the parser's lexer and compiler
	FMathExpEvaluator();

	// Evaluate the given expression, resulting in either a double value, or an error
	TValueOrError<double, FExpressionError> Evaluate(const TCHAR* const InExpression) const;

	// bind to this to be able to use variables
	FJEXVGetVar OnGetVar;
	FJEXVSetVar OnSetVar;
	FJEXVSetVarId OnSetVarId;

private:
	TOptional<FExpressionError> ConsumeVarName(FExpressionTokenConsumer& Consumer) const;
	TOptional<FExpressionError> ConsumeStr(FExpressionTokenConsumer& Consumer) const;
	void SetVarId(const double NameId, const double Val) const;
	void SetVar(const FString& NameId, const double Val) const;

	FTokenDefinitions TokenDefinitions;
	FExpressionGrammar Grammar;
	FOperatorJumpTable JumpTable;
};
