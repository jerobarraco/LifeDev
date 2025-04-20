// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

// ReSharper disable CppFunctionalStyleCast
// ReSharper disable CppCStyleCast
#include "MathExpEvaluator.h"

#include "Math/BasicMathExpressionEvaluator.h"
#include "Misc/AutomationTest.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/ExpressionParser.h"
#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "JMathExpEvaluator"

// TODO cleanup names
DEFINE_LOG_CATEGORY_STATIC(LogJEvalExp, Log, Log)

namespace ExpressionParser {
	const TCHAR* const FSaturate::Moniker = TEXT("sat");
	const TCHAR* const FAbsolute::Moniker = TEXT("abs");
	const TCHAR* const FFloor::Moniker = TEXT("floor");
	const TCHAR* const FCeil::Moniker = TEXT("ceil");
	const TCHAR* const FTrunc::Moniker = TEXT("trunc");
	const TCHAR* const FFrac::Moniker = TEXT("frac");
	const TCHAR* const FPick::Moniker = TEXT("pick");
	
	const TCHAR* const FRand::Moniker = TEXT("?");
	const TCHAR* const FNot::Moniker = TEXT("!");
	const TCHAR* const FAnd::Moniker = TEXT("&");
	const TCHAR* const FOr::Moniker = TEXT("|");
	const TCHAR* const FXor::Moniker = TEXT("$");
	const TCHAR* const FGreatThan::Moniker = TEXT(">");
	const TCHAR* const FLessThan::Moniker = TEXT("<");
	const TCHAR* const FEquals::Moniker = TEXT("=");
	const TCHAR* const FSet::Moniker = TEXT(":");
	const TCHAR* const FItem::Moniker = TEXT(",");
	const TCHAR* const FIndex::Moniker = TEXT("["); // todo find better
}
DEFINE_EXPRESSION_NODE_TYPE(FString, 0x8444A8A3, 0x19AE4E13, 0xBCFA75EE, 0x39982B99)

namespace JMathExp {
	static const TCHAR PropertyBreakingChars[] = { '|', '=', '&', '>', '<', '!', '+', '-', '*', '/', '\t', '(', ')' };// ' ',
	
	static inline bool _IsFalse(const double A) {
		return A <= 0 || FMath::IsNearlyZero(A);
	}
}

FMathExpEvaluator::FMathExpEvaluator() {
	using namespace ExpressionParser;

	// reusing a bunch from basicmath...
	TokenDefinitions.IgnoreWhitespace();
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSubExpressionStart>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSubExpressionEnd>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPlus>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FMinus>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FStar>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FForwardSlash>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPercent>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSquareRoot>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSaturate>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FAbsolute>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FFloor>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FCeil>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FTrunc>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FFrac>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPick>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FPower>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FRand>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FNot>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FAnd>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FOr>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FXor>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FGreatThan>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FLessThan>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FEquals>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FSet>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FItem>);
	TokenDefinitions.DefineToken(&ConsumeSymbol<FIndex>);
	TokenDefinitions.DefineToken(&ConsumeLocalizedNumberWithAgnosticFallback);

	// replace strings with values
	TokenDefinitions.DefineToken([this](FExpressionTokenConsumer& Consumer) -> TOptional<FExpressionError> {
		return this->ConsumeVarName(Consumer);
	});
	// replace strings with fstrings
	TokenDefinitions.DefineToken([this](FExpressionTokenConsumer& Consumer) -> TOptional<FExpressionError> {
		return this->ConsumeStr(Consumer);
	});

	Grammar.DefineGrouping<FSubExpressionStart, FSubExpressionEnd>();
	
	Grammar.DefinePreUnaryOperator<FPlus>();
	Grammar.DefinePreUnaryOperator<FMinus>();
	Grammar.DefinePreUnaryOperator<FSquareRoot>();
	Grammar.DefinePreUnaryOperator<FNot>();
	Grammar.DefinePreUnaryOperator<FSaturate>();
	Grammar.DefinePreUnaryOperator<FAbsolute>();
	Grammar.DefinePreUnaryOperator<FPick>();
	Grammar.DefinePreUnaryOperator<FFloor>();
	Grammar.DefinePreUnaryOperator<FCeil>();
	Grammar.DefinePreUnaryOperator<FTrunc>();
	Grammar.DefinePreUnaryOperator<FFrac>();

	// Left-to-right evaluation is required for non-commutative binary operations, and a reasonable default for commutative ones too.
	Grammar.DefineBinaryOperator<FPower>(3);
	Grammar.DefineBinaryOperator<FRand>(3);
	Grammar.DefineBinaryOperator<FStar>(4, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FForwardSlash>(4, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FPercent>(4, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FPlus>(5, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FMinus>(5, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FItem>(6, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FIndex>(6, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FGreatThan>(6, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FLessThan>(6, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FEquals>(6, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FAnd>(7, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FOr>(7,  EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FXor>(7, EAssociativity::LeftToRight);
	Grammar.DefineBinaryOperator<FSet>(11, EAssociativity::LeftToRight);

	JumpTable.MapPreUnary<FPlus>([](const double N) {
		UE_LOG(LogJEvalExp, Log, TEXT("Plus A=%.5f"), N);
		return N;
	});
	JumpTable.MapPreUnary<FMinus>([](const double N) {
		UE_LOG(LogJEvalExp, Log, TEXT("minus A=%.5f"), N);
		return -N;
	});
	JumpTable.MapPreUnary<FSquareRoot>([](const double A)		{ 
		UE_LOG(LogJEvalExp, Log, TEXT("sqr A=%.5f"), A);
		return double(FMath::Sqrt(A));
	});
	JumpTable.MapPreUnary<FSaturate>([](const double A){
		const double B = FMath::Clamp(A, double(0), double(1));
		UE_LOG(LogJEvalExp, Log, TEXT("Saturate A=%.5f B=%.5f"), A, B);
		return B;
	});
	JumpTable.MapPreUnary<FAbsolute>([](const double A) {
		const double B = FMath::Abs(A); 
		UE_LOG(LogJEvalExp, Log, TEXT("Absolute A=%.5f B=%.5f"), A, B);
		return double(B);
	});
	JumpTable.MapPreUnary<FNot>([](const double A) {
		UE_LOG(LogJEvalExp, Log, TEXT("Not A=%.5f"), A);
		return double(JMathExp::_IsFalse(A) ? 1.0 : 0.0);
	});

	JumpTable.MapPreUnary<FPick>([](const TArray<double>& A) -> FExpressionResult {
		const int32 Num = A.Num();
		const int32 I = FMath::RandRange(0, Num-1);
		UE_LOG(LogJEvalExp, Log, TEXT("Pick A[%i] i=%i"), Num, I);
		if (UNLIKELY(Num==0)) return MakeError(LOCTEXT("Pick", "List is empty"));
		if (Num==1) return MakeValue(A[0]);

		return MakeValue(A[I]);
	});
	JumpTable.MapPreUnary<FFloor>([](const double A) {
		UE_LOG(LogJEvalExp, Log, TEXT("Floor A=%.5f"), A);
		return FMath::Floor(A);
	});
	JumpTable.MapPreUnary<FCeil>([](const double A) {
		UE_LOG(LogJEvalExp, Log, TEXT("Ceil A=%.5f"), A);
		return FMath::CeilToDouble(A);
	});
	JumpTable.MapPreUnary<FTrunc>([](const double A) {
		UE_LOG(LogJEvalExp, Log, TEXT("Trunc A=%.5f"), A);
		return FMath::TruncToDouble(A);
	});
	JumpTable.MapPreUnary<FFrac>([](const double A) {
		UE_LOG(LogJEvalExp, Log, TEXT("Frac A=%.5f"), A);
		return FMath::Frac(A);
	});

	JumpTable.MapBinary<FPlus>([](const double A, const double B)	{ return A + B; });
	JumpTable.MapBinary<FMinus>([](const double A, const double B)	{ return A - B; });
	JumpTable.MapBinary<FStar>([](const double A, const double B)	{ return A * B; });
	JumpTable.MapBinary<FPower>([](const double A, const double B)	{ return double(FMath::Pow(A, B)); });
	JumpTable.MapBinary<FForwardSlash>([](const double A, const double B) -> FExpressionResult  {
		if (UNLIKELY(B == 0)) return MakeError(LOCTEXT("DivisionByZero", "Division by zero"));
		return MakeValue(A / B);
	});
	JumpTable.MapBinary<FPercent>([](const double A, const double B) -> FExpressionResult { /// returns expression result since it can return an error
		if (UNLIKELY(B == 0)) return MakeError(LOCTEXT("ModZero", "Modulo zero"));
		return MakeValue(FMath::Fmod(A, B)); // todo fix this on the epic's repo
	});
	JumpTable.MapBinary<FRand>([](const double A, const double B) -> double {
		return FMath::FRandRange(A, B);
	});
	JumpTable.MapBinary<FAnd>([](const double A, const double B) -> double {
		return JMathExp::_IsFalse(A) ? 0.0 : B;
	});
	JumpTable.MapBinary<FOr>([](const double A, const double B) -> double {
		return JMathExp::_IsFalse(A) ? B : A;
	});
	JumpTable.MapBinary<FXor>([](const double A, const double B) -> double {
		const bool FalseA = JMathExp::_IsFalse(A);
		const bool FalseB = JMathExp::_IsFalse(B);
		const bool Same = FalseA == FalseB;
		return Same ? 0.0: 1.0;
	});
	JumpTable.MapBinary<FGreatThan>([](const double A, const double B) -> double {
		const bool True = A>B;
		return True ? 1.0: 0.0;
	});
	JumpTable.MapBinary<FLessThan>([](const double A, const double B) -> double {
		const bool True = A<B;
		return True ? 1.0: 0.0;
	});
	JumpTable.MapBinary<FEquals>([](const double A, const double B) -> double {
		const bool True = FMath::IsNearlyEqual(A, B);
		return True ? 1.0: 0.0;
	});
	JumpTable.MapBinary<FSet>([this](const double A, const double B) -> double {
		UE_LOG(LogJEvalExp, Log, TEXT("%hs FSet: A=%.5f B=%.5f"), __func__, A, B);
		SetVarId(A, B);
		return B;
	});
	JumpTable.MapBinary<FSet>([this](const FString& A, const double B) -> double {
		UE_LOG(LogJEvalExp, Log, TEXT("%hs FSet: A=%s B=%.5f"), __func__, *A, B);
		SetVar(A, B);
		return B;
	});

	JumpTable.MapBinary<FItem>([this](const double A, const double B) -> TArray<double> {
		UE_LOG(LogJEvalExp, Log, TEXT("%hs Fitem::Base: A=%.5f B=%.5f"), __func__, A, B);
		
		return TArray<double>({A, B});
	});
	JumpTable.MapBinary<FItem>([this](const TArray<double>& A, const double B) -> TArray<double> {
		UE_LOG(LogJEvalExp, Log, TEXT("%hs Fitem::Add: A[%i] B=%.5f"), __func__, A.Num(), B);
		TArray<double> Ret(A);
		Ret.Add(B);
		return Ret;
	});
	JumpTable.MapBinary<FIndex>([this](const TArray<double>& A, const double B) -> FExpressionResult {
		const int32 I = FMath::TruncToInt32(B);
		const int32 N = A.Num();
		UE_LOG(LogJEvalExp, Log, TEXT("%hs FIndex A[%i] B=%.5f I=%i"), __func__, N, B, I);
		if (I<0||I>=N) return MakeError(LOCTEXT("IndexOutOfBounds", "Index <0 or >=Number (out of bounds)."));
		return MakeValue(A[I]);
	});
}

TValueOrError<double, FExpressionError> FMathExpEvaluator::Evaluate(
const TCHAR* const InExpression) const {
	using namespace ExpressionParser;

	TValueOrError<TArray<FExpressionToken>, FExpressionError> LexResult = ExpressionParser::Lex(InExpression, TokenDefinitions);
	if (UNLIKELY(!LexResult.IsValid()))
		return MakeError(LexResult.StealError());

	TArray<FExpressionToken> Tokens = LexResult.StealValue();
	TValueOrError<TArray<FCompiledToken>, FExpressionError> CompilationResult =
		ExpressionParser::Compile(MoveTemp(Tokens), Grammar);
	if (UNLIKELY(!CompilationResult.IsValid())) return MakeError(CompilationResult.StealError());

	const TOperatorEvaluationEnvironment<> Env(JumpTable, nullptr);
	TValueOrError<FExpressionNode, FExpressionError> Result =
		ExpressionParser::Evaluate(CompilationResult.GetValue(), Env);
	if (UNLIKELY(!Result.IsValid())) return MakeError(Result.GetError());

	const auto& Node = Result.GetValue();

	if (const double* Numeric = Node.Cast<double>()) return MakeValue(*Numeric);

	return MakeError(LOCTEXT("UnrecognizedResult", "Unrecognized result returned from expression"));
}

TOptional<FExpressionError> FMathExpEvaluator::ConsumeVarName(FExpressionTokenConsumer& Consumer) const {
	FString VarName;
	bool IsAtStart = true;
	static constexpr TCHAR OpenC = '{';
	static constexpr TCHAR CloseC = '}';
	
	TOptional<FStringToken> StringToken = Consumer.GetStream().ParseToken(
	[&VarName, &IsAtStart](const TCHAR InC){
		if (UNLIKELY(IsAtStart)) {
			IsAtStart = false;
			return InC == OpenC ? EParseState::Continue : EParseState::Cancel; // not quoted, we don't want.
		}
	
		if (UNLIKELY(InC == CloseC))
			return EParseState::StopAfter;

		VarName.AppendChar(InC);
		return EParseState::Continue;
	});

	// Do i need this?
	VarName.ReplaceEscapedCharWithCharInline();

	// so basically i consume the string and turn it into a number, and then pretend it's that value.
	// which it actually is.
	if (LIKELY(StringToken.IsSet())) {
		const double Val = LIKELY(OnGetVar.IsBound()) ? OnGetVar.Execute(FName(VarName)): 0;
		Consumer.Add(StringToken.GetValue(), FExpressionNode(Val));
	}

	return TOptional<FExpressionError>();
}

TOptional<FExpressionError> FMathExpEvaluator::ConsumeStr(FExpressionTokenConsumer& Consumer) const {
	FString VarName;
	bool IsAtStart = true;
	static constexpr TCHAR OpenC = '"';
	static constexpr TCHAR CloseC = '"';
	
	TOptional<FStringToken> StringToken = Consumer.GetStream().ParseToken(
	[&VarName, &IsAtStart](const TCHAR InC){
		if (UNLIKELY(IsAtStart)) {
			IsAtStart = false;
			return InC == OpenC ? EParseState::Continue : EParseState::Cancel; // not quoted, we don't want.
		}
	
		if (UNLIKELY(InC == CloseC))
			return EParseState::StopAfter;

		VarName.AppendChar(InC);
		return EParseState::Continue;
	});

	// Do i need this?
	VarName.ReplaceEscapedCharWithCharInline();

	// so basically i consume the string and turn it into a number, and then pretend it's that value.
	// which it actually is.
	if (LIKELY(StringToken.IsSet()))
		Consumer.Add(StringToken.GetValue(), FExpressionNode(VarName));

	return TOptional<FExpressionError>();
}

void FMathExpEvaluator::SetVarId(const double NameId, const double Val) const {
	if (UNLIKELY(!OnSetVarId.IsBound())) return;
	OnSetVarId.Execute(NameId, Val);
}

void FMathExpEvaluator::SetVar(const FString& NameId, const double Val) const {
	if (UNLIKELY(!OnSetVar.IsBound())) return;
	OnSetVar.Execute(NameId, Val);
}

#undef LOCTEXT_NAMESPACE

// see also basicmathexpressionevaluator. thanks tim team.