
// ReSharper disable CppFunctionalStyleCast
// ReSharper disable CppCStyleCast
#include "MathExpEvaluator.h"

#include "Math/BasicMathExpressionEvaluator.h"
#include "Misc/AutomationTest.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/ExpressionParser.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "JMathExpEvaluator"

namespace JMathExp {
	// const TCHAR* const FSubExpressionStart::Moniker = TEXT("(");
	// const TCHAR* const FSubExpressionEnd::Moniker = TEXT(")");
	// const TCHAR* const FPlus::Moniker = TEXT("+");
	// const TCHAR* const FPlusEquals::Moniker = TEXT("+=");
	// const TCHAR* const FMinus::Moniker = TEXT("-");
	// const TCHAR* const FMinusEquals::Moniker = TEXT("-=");
	// const TCHAR* const FStar::Moniker = TEXT("*");
	// const TCHAR* const FStarEquals::Moniker = TEXT("*=");
	// const TCHAR* const FForwardSlash::Moniker = TEXT("/");
	// const TCHAR* const FForwardSlashEquals::Moniker = TEXT("/=");
	// const TCHAR* const FPercent::Moniker = TEXT("%");
	// const TCHAR* const FSquareRoot::Moniker = TEXT("sqrt");
	// const TCHAR* const FPower::Moniker = TEXT("^");

	const FDecimalNumberFormattingRules& GetLocalizedNumberFormattingRules() {
		bool bShouldUseLocalizedNumericInput = false;
		GConfig->GetBool(TEXT("Internationalization"), TEXT("ShouldUseLocalizedNumericInput"), bShouldUseLocalizedNumericInput, GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni);
		return bShouldUseLocalizedNumericInput 
			? FInternationalization::Get().GetCurrentLocale()->GetDecimalNumberFormattingRules() 
			: FastDecimalFormat::GetCultureAgnosticFormattingRules();
	}

	TOptional<FStringToken> ParseNumberWithFallback(const FTokenStream& InStream, const FDecimalNumberFormattingRules& InPrimaryFormattingRules, const FDecimalNumberFormattingRules& InFallbackFormattingRules, FStringToken* Accumulate, double* OutValue)
	{
		// Attempt to parse a number from the string
		// This call will return false if there is some other data after the number, which is why we check the parsed length instead
		double PrimaryValue = 0.0;
		int32 PrimaryParsedLen = 0;
		FastDecimalFormat::StringToNumber(InStream.GetRead(), UE_PTRDIFF_TO_INT32(InStream.GetEnd() - InStream.GetRead()), InPrimaryFormattingRules, FNumberParsingOptions::DefaultNoGrouping(), PrimaryValue, &PrimaryParsedLen);

		double FallbackValue = 0.0;
		int32 FallbackParsedLen = 0;
		FastDecimalFormat::StringToNumber(InStream.GetRead(), UE_PTRDIFF_TO_INT32(InStream.GetEnd() - InStream.GetRead()), InFallbackFormattingRules, FNumberParsingOptions::DefaultNoGrouping(), FallbackValue, &FallbackParsedLen);

		// We take whichever value parsed the most text from the string
		if (FallbackParsedLen <= PrimaryParsedLen)
		{
			if (OutValue)
			{
				*OutValue = PrimaryValue;
			}

			return PrimaryParsedLen > 0 ? InStream.GenerateToken(PrimaryParsedLen) : TOptional<FStringToken>();
		}
		else
		{
			if (OutValue)
			{
				*OutValue = FallbackValue;
			}

			return FallbackParsedLen > 0 ? InStream.GenerateToken(FallbackParsedLen) : TOptional<FStringToken>();
		}
	}

	TOptional<FStringToken> ParseNumberWithRules(const FTokenStream& InStream, const FDecimalNumberFormattingRules& InFormattingRules, FStringToken* Accumulate, double* OutValue)
	{
		// Attempt to parse a number from the string
		// This call will return false if there is some other data after the number, which is why we check the parsed length instead
		double Value = 0.0;
		int32 ParsedLen = 0;
		FastDecimalFormat::StringToNumber(InStream.GetRead(), UE_PTRDIFF_TO_INT32(InStream.GetEnd() - InStream.GetRead()), InFormattingRules, FNumberParsingOptions::DefaultNoGrouping(), Value, &ParsedLen);

		if (OutValue)
		{
			*OutValue = Value;
		}

		return ParsedLen > 0 ? InStream.GenerateToken(ParsedLen) : TOptional<FStringToken>();
	}

	TOptional<FStringToken> ParseLocalizedNumberWithAgnosticFallback(const FTokenStream& InStream, FStringToken* Accumulate, double* OutValue)
	{
		return ParseNumberWithFallback(InStream, GetLocalizedNumberFormattingRules(), FastDecimalFormat::GetCultureAgnosticFormattingRules(), Accumulate, OutValue);
	}

	TOptional<FStringToken> ParseLocalizedNumber(const FTokenStream& InStream, FStringToken* Accumulate, double* OutValue)
	{
		return ParseNumberWithRules(InStream, GetLocalizedNumberFormattingRules(), Accumulate, OutValue);
	}

	TOptional<FStringToken> ParseNumber(const FTokenStream& InStream, FStringToken* Accumulate, double* OutValue)
	{
		return ParseNumberWithRules(InStream, FastDecimalFormat::GetCultureAgnosticFormattingRules(), Accumulate, OutValue);
	}

	TOptional<FExpressionError> ConsumeNumberWithRules(FExpressionTokenConsumer& Consumer, const FDecimalNumberFormattingRules& InFormattingRules)
	{
		auto& Stream = Consumer.GetStream();

		double Value = 0.0;
		TOptional<FStringToken> Token = ParseNumberWithRules(Stream, InFormattingRules, nullptr, &Value);

		if (Token.IsSet())
		{
			Consumer.Add(Token.GetValue(), FExpressionNode(Value));
		}

		return TOptional<FExpressionError>();
	}

	TOptional<FExpressionError> ConsumeLocalizedNumberWithAgnosticFallback(FExpressionTokenConsumer& Consumer)
	{
		auto& Stream = Consumer.GetStream();

		double Value = 0.0;
		TOptional<FStringToken> Token = ParseLocalizedNumberWithAgnosticFallback(Stream, nullptr, &Value);

		if (Token.IsSet())
		{
			Consumer.Add(Token.GetValue(), FExpressionNode(Value));
		}

		return TOptional<FExpressionError>();
	}

	TOptional<FExpressionError> ConsumeLocalizedNumber(FExpressionTokenConsumer& Consumer)
	{
		return ConsumeNumberWithRules(Consumer, GetLocalizedNumberFormattingRules());
	}

	TOptional<FExpressionError> ConsumeNumber(FExpressionTokenConsumer& Consumer)
	{
		return ConsumeNumberWithRules(Consumer, FastDecimalFormat::GetCultureAgnosticFormattingRules());
	}
}

FMathExpEvaluator::FMathExpEvaluator() {
	using namespace ExpressionParser;

	TokenDefinitions.IgnoreWhitespace();
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSubExpressionStart>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSubExpressionEnd>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPlusEquals>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FMinusEquals>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FStarEquals>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FForwardSlashEquals>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPlus>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FMinus>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FStar>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FForwardSlash>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPercent>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSquareRoot>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPower>);
	TokenDefinitions.DefineToken(&ConsumeLocalizedNumberWithAgnosticFallback);

	Grammar.DefineGrouping<FSubExpressionStart, FSubExpressionEnd>();
	Grammar.DefinePreUnaryOperator<FPlus>();
	Grammar.DefinePreUnaryOperator<FMinus>();
	Grammar.DefinePreUnaryOperator<FSquareRoot>();

	// Left-to-right evaluation is required for non-commutative binary operations, and a reasonable default for commutative ones too.
	Grammar.DefineBinaryOperator<FPlus>(5, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FMinus>(5, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FStar>(4, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FForwardSlash>(4, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FPercent>(4, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FPower>(3);

	JumpTable.MapPreUnary<FPlus>([](double N)			{ return N; });
	JumpTable.MapPreUnary<FMinus>([](double N)			{ return -N; });
	JumpTable.MapPreUnary<FSquareRoot>([](double A)		{ return double(FMath::Sqrt(A)); });

	JumpTable.MapBinary<FPlus>([](double A, double B)	{ return A + B; });
	JumpTable.MapBinary<FMinus>([](double A, double B)	{ return A - B; });
	JumpTable.MapBinary<FStar>([](double A, double B)	{ return A * B; });
	JumpTable.MapBinary<FPower>([](double A, double B)	{ return double(FMath::Pow(A, B)); });

	JumpTable.MapBinary<FForwardSlash>([](double A, double B) -> FExpressionResult {
		if (B == 0)
		{
			return MakeError(LOCTEXT("DivisionByZero", "Division by zero"));
		}

		return MakeValue(A / B);
	});
	JumpTable.MapBinary<FPercent>([](double A, double B) -> FExpressionResult {
		if (B == 0)
		{
			return MakeError(LOCTEXT("ModZero", "Modulo zero"));
		}

		return MakeValue(double(FMath::Fmod(A, B))); // todo fix this on the epic's repo
	});
}

TValueOrError<double, FExpressionError> FMathExpEvaluator::Evaluate(const TCHAR* InExpression, double InExistingValue) const
{
	using namespace ExpressionParser;

	// TODO ExpressionParser or JMathExp?
	TValueOrError<TArray<FExpressionToken>, FExpressionError> LexResult = ExpressionParser::Lex(InExpression, TokenDefinitions);
	if (!LexResult.IsValid())
	{
		return MakeError(LexResult.StealError());
	}

	// Handle the += and -= tokens.
	TArray<FExpressionToken> Tokens = LexResult.StealValue();
	if (Tokens.Num())
	{
		FStringToken Context = Tokens[0].Context;
		const FExpressionNode& FirstNode = Tokens[0].Node;
		bool WasOpAssign = true;

		if (FirstNode.Cast<FPlusEquals>())
		{
			Tokens.Insert(FExpressionToken(Context, FPlus()), 0);
		}
		else if (FirstNode.Cast<FMinusEquals>())
		{
			Tokens.Insert(FExpressionToken(Context, FMinus()), 0);
		}
		else if (FirstNode.Cast<FStarEquals>())
		{
			Tokens.Insert(FExpressionToken(Context, FStar()), 0);
		}
		else if (FirstNode.Cast<FForwardSlashEquals>())
		{
			Tokens.Insert(FExpressionToken(Context, FForwardSlash()), 0);
		}
		else
		{
			WasOpAssign = false;
		}

		if (WasOpAssign)
		{
			Tokens.Insert(FExpressionToken(Context, InExistingValue), 0);
			Tokens.RemoveAt(2, EAllowShrinking::No);
		}
	}

	TValueOrError<TArray<FCompiledToken>, FExpressionError> CompilationResult = ExpressionParser::Compile(MoveTemp(Tokens), Grammar);
	if (!CompilationResult.IsValid())
	{
		return MakeError(CompilationResult.StealError());
	}

	TOperatorEvaluationEnvironment<> Env(JumpTable, nullptr);
	TValueOrError<FExpressionNode, FExpressionError> Result = ExpressionParser::Evaluate(CompilationResult.GetValue(), Env);
	if (!Result.IsValid())
	{
		return MakeError(Result.GetError());
	}

	auto& Node = Result.GetValue();

	if (const auto* Numeric = Node.Cast<double>())
	{
		return MakeValue(*Numeric);
	}

	return MakeError(LOCTEXT("UnrecognizedResult", "Unrecognized result returned from expression"));
}


#undef LOCTEXT_NAMESPACE
