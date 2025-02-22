// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "Diags.h"

#include "EvalMath.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiags, Log, Log);

UDiags* UDiags::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	
	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	UDiags* const D = W->GetSubsystem<UDiags>();
	return UNLIKELY(IsValid(D)) ? D : nullptr;
}

void UDiags::AddDiag(const FDialog& Diag) {
	Pending.Add(Diag);
	ShowNext();
}

bool UDiags::AddDiagId(const FName& Row, const bool Warn) {
	UE_LOG(LogDiags, Log, TEXT("%hs: row=%s, warn=%i"), __func__, *Row.ToString(), Warn);
	FDialog OutDialog; FDialogChar OutChar;
	const bool Ok = GetDiag(Row, OutDialog, OutChar, Warn);
	if (UNLIKELY(!Ok)) return false;

	double Res = 0;
	if (UNLIKELY(!CheckCondition(OutDialog.Condition, Res))) {
		UE_LOG(LogDiags, Log, TEXT("%hs: Condition not met. row=%s condition=%s"),
			__func__, *Row.ToString(), *OutDialog.Condition);
		return false;
	}

	OnAdd.Broadcast(Row, OutDialog); // before addDiag since it will trigger all sorts of other stuff.
	AddDiag(OutDialog);
	return true;
}

bool UDiags::AddId(const FName& Row) {
	if (UNLIKELY(Row.IsNone())) return false;

	// attempt to add a sequence (can be random) (could trigger another call to AddId)
	if (AddSeqId(Row, false)) return true;

	// otherwise attempt a dialog
	if (LIKELY(AddDiagId(Row, false))) return true;

	// this also would capture a sequence that is empty or the ids are none.
	UE_LOG(LogDiags, Warning,
		TEXT("%hs: Could not find dialog nor sequence with the id=%s. "
			"Or the sequence was empty or invalid."), __func__, *Row.ToString());
	return false;
}

bool UDiags::AddIdMany(const TArray<FName>& Rows) {
	const int32 Num = Rows.Num();
	bool Success = Num > 0; // return false if nothing was added
	for (int32 i = 0; i < Num; ++i) {
		// done this way on purpose, so if a sequence adds another sequence
		// it will get "expanded" as a parenthesis.
		// AddId is safe to call many times since ShowNext has a flag
		const FName& Row = Rows[i];
		// notice this is recursive. that's on purpose, but be careful.
		const bool Ok = AddId(Row);

		Success = Success && Ok; // ok will set it to false (faster than branch)
	}

	return Success;
}

bool UDiags::AddSeq(const FDialogSequence& Seq) {
	const int32 Num = Seq.DiagRows.Num();
	if (Num <= 0) return false;

	const TArray<FName>& Rows = Seq.DiagRows;
	const int32 DiagNum = Rows.Num();
	double Res;
	const bool CondOk = CheckCondition(Seq.Condition, Res);
	if (Seq.Modifier == ESeqMod::NORMAL) {
		if (UNLIKELY(!CondOk)) return false;
	} else if (Seq.Modifier == ESeqMod::RANDOM) {
		const int32 i = FMath::RandRange(0, Num -1);
		return AddId(Rows[i]);
	} else if (Seq.Modifier == ESeqMod::SELECT_LOOP) {
		const int32 i = FMath::RoundToInt32(FMath::Modulo(Res, DiagNum));
		const FName DiagRow = Rows[i];
		return AddId(DiagRow);
	} else if (Seq.Modifier == ESeqMod::SELECT_LOOP) {
		const int32 i = FMath::Clamp(Res, 0, DiagNum);
		const FName DiagRow = Rows[i];
		return AddId(DiagRow);
	} // else wtf

	return AddIdMany(Rows);
}

bool UDiags::AddSeqId(const FName& RowName, const bool Warn) {
	FDialogSequence Seq;
	const bool Ok = GetSeq(RowName, Seq, Warn);
	if (!Ok) return false;

	const int32 DiagNum = Seq.DiagRows.Num();
	const FString RowNameStr = RowName.ToString();
	// prevent recursion. Notice this doesn't fix cyclic sequences. no simple way to tell either.
	// not a priority either.
	// int because num-1 can be negative. iterating backwards to be able to remove easily.
	for (int32 i = DiagNum -1; i>=0; --i) {
		const FName& DiagName = Seq.DiagRows[i];
		if (DiagName != RowName) continue;

		UE_LOG(LogDiags, Warning, TEXT("Attempted to add a recursive sequence. Seq=%s diag=%s"),
			*RowNameStr, *DiagName.ToString());
		Seq.DiagRows.RemoveAt(i); // this is safe only because GetSeq returns a copy. :)
	}

	if (DiagNum < 1) {
		UE_LOG(LogDiags, Warning, TEXT("%hs: Sequence is empty! (after removing loops). Skip."
			" row=%s"), __func__, *RowName.ToString());
		return false;
	}

	// double CondRes = 0;
	// const bool CondOk = CheckCondition(Seq.Condition, CondRes);
	if (RowNameStr.EndsWith("!")) {
		UE_LOG(LogDiags, Warning, TEXT("%hs, ? is deprecated. use select clamp."), __func__);
		Seq.Modifier = ESeqMod::SELECT_CLAMP;
	}

	if (RowNameStr.EndsWith("?")) {
		UE_LOG(LogDiags, Warning, TEXT("%hs, ? is deprecated. use select clamp or loop modifier."), __func__);
		Seq.Modifier = ESeqMod::SELECT_LOOP;
	}

	// add random or regular accordingly. if it ends with * it's ALWAYS random
	if (RowNameStr.EndsWith("*")){
		UE_LOG(LogDiags, Warning, TEXT("%hs Using * suffix. please use modifier. this is going to get removed soon."), __func__);
		Seq.Modifier= ESeqMod::RANDOM;
	}

	return AddSeq(Seq);
}

bool UDiags::AddRnd(const FDialogSequence& Seq) {
	const int32 Num = Seq.DiagRows.Num();
	if (Num <= 0) return false;

	const int32 i = FMath::RandRange(0, Num -1);
	return AddId(Seq.DiagRows[i]);
}

void UDiags::DiagDone() {
	IsShowing = false;
	ShowNext();
}

void UDiags::SetData(
	UDataTable* const AllDiags, UDataTable* const AllChars, UDataTable* const AllSeqs) {
	Diags = IsValid(AllDiags)? AllDiags : nullptr;
	Chars = IsValid(AllChars)? AllChars: nullptr;
	Seqs = IsValid(AllSeqs)? AllSeqs: nullptr;
}

void UDiags::Init() {
	Eval = UEvalMath::Instance(this);
}

void UDiags::DeInit() {
	Diags = nullptr;
	Chars = nullptr;
	Seqs = nullptr;
	Eval = nullptr;
}

bool UDiags::GetDiag(
const FName& RowName, FDialog& OutRow, FDialogChar& OutChar, const bool Warn) const {
	if (UNLIKELY(RowName.IsNone())) return false;
	if (UNLIKELY(!IsValid(Diags))) return false;

	const FDialog* const Row = Diags->FindRow<FDialog>(RowName, TEXT(""), Warn);
	if (UNLIKELY(!Row)) {
		UE_LOG(LogDiags, Verbose, TEXT("Could not find dialog for row=%s"), *RowName.ToString());
		return false;
	}

	OutRow = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	GetChar(OutRow.CharRow, OutChar); // ignore if the char is not found for the result, we only care about Diags
	return true;
}

bool UDiags::GetChar(const FName& RowName, FDialogChar& OutChar, const bool Warn) const {
	if (UNLIKELY(RowName.IsNone())) return false;
	if (UNLIKELY(!IsValid(Chars))) return false;

	const FDialogChar* const Row = Chars->FindRow<FDialogChar>(RowName, TEXT(""), Warn);
	if (UNLIKELY(!Row)) {
		UE_LOG(LogDiags, Warning, TEXT("Could not find character for row=%s"), *RowName.ToString());
		return false;
	}

	OutChar = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	return true;
}

bool UDiags::GetSeq(const FName& RowName, FDialogSequence& OutSeq, const bool Warn) const {
	if (UNLIKELY(RowName.IsNone())) return false;
	if (UNLIKELY(!IsValid(Seqs))) return false;

	const FDialogSequence* const Row =
		Seqs->FindRow<FDialogSequence>(RowName, TEXT(""), Warn);
	if (UNLIKELY(!Row)) {
		UE_LOG(LogDiags, Verbose, TEXT("Could not find sequence for row=%s"), *RowName.ToString());
		return false;
	}

	OutSeq = *Row; // here im copying, which s-u-x. but blueprints won't take a pointer. also it's safer.
	return true;
}

void UDiags::ShowNext() {
	if (UNLIKELY(IsShowing)) return;
	// this is important for stop to work correctly. and also in general
	IsShowing = true;
	
	if (UNLIKELY(Pending.IsEmpty())) {
		Stop();
		return;
	}

	// Do NOT get a reference here,
	// since we will remove later, it will actually return weird data.
	FDialog Diag = Pending[0];
	Pending.RemoveAt(0);

	OnShow.Broadcast(Diag);
}

void UDiags::Stop() {
	if (UNLIKELY(!IsShowing)) return;
	IsShowing = false;

	OnDone.Broadcast();
}

bool UDiags::CheckCondition(const FString& Expression, double& Res) const {
	Res = 0;
	if (UNLIKELY(!Eval)) return false;

	bool Ok = false; 
	Res = Eval->Eval(Expression, Ok);
	return Ok;
}
