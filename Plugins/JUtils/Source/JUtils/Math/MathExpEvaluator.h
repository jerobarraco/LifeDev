
#pragma once

#include "CoreTypes.h"
#include "Internationalization/FastDecimalFormat.h"
#include "Misc/ExpressionParserTypes.h"
#include "Misc/Optional.h"
#include "Templates/ValueOrError.h"

// this way it doesn't need the .generated. which needs a uclass. which we don't have.
DECLARE_DELEGATE_RetVal_OneParam(double, FJEXVGetVar, const FName);

struct FDecimalNumberFormattingRules;

DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FSaturate, 0x6F88756B, 0xF9234263, 0x9B13614F, 0x27060734)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FAbsolute,  0x6F88756B, 0xF9234263, 0x9B13614F, 0x21060726)
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FRand, "JMathRand")
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FNot, "JMathNot")
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FAnd, "JMathAnd")
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FOr, "JMathOr")
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FXor, "JMathXor")
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FGreatThan, "JMathGT")
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FLessThan, "JMathLT")
DEFINE_EXPRESSION_OPERATOR_NODE(JUTILS_API, FEquals, "JMathEq")

// A basic math expression evaluator. with variables. and logic. and not gambling.
class JUTILS_API FMathExpEvaluator {
public:
	// Constructor that sets up the parser's lexer and compiler */
	FMathExpEvaluator();

	// Evaluate the given expression, resulting in either a double value, or an error */
	TValueOrError<double, FExpressionError> Evaluate(const TCHAR* InExpression, double InExistingValue = 0) const;

	FJEXVGetVar OnGetVar;

private:
	TOptional<FExpressionError> ConsumePropertyName(FExpressionTokenConsumer& Consumer) const;

	FTokenDefinitions TokenDefinitions;
	FExpressionGrammar Grammar;
	FOperatorJumpTable JumpTable;
};
