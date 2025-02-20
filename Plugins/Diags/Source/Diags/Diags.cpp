// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "Diags.h"

#include "JUtils/Misc/JUtilsMisc.h"

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

	float Res = 0;
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
	const TArray<FName>& Rows = Seq.DiagRows;
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

	float CondRes = 0;
	const bool CondOk = UNLIKELY(CheckCondition(Seq.Condition, CondRes));
	if (RowNameStr.EndsWith("!")) {
		UE_CLOG(DiagNum>2, LogDiags, Warning, TEXT("%hs: More than 2 options. Will ignore the rest."
			" Row=%s Condition=%s"), __func__, *RowName.ToString(), *Seq.Condition);
		UE_CLOG(DiagNum<2, LogDiags, Warning, TEXT("%hs: Less than 2 options. Will clamp."
			" Row=%s Condition=%s"), __func__, *RowName.ToString(), *Seq.Condition);
		
		const FName DiagRow = Seq.DiagRows[CondOk || DiagNum <2 ? 0: 1];
		return AddId(DiagRow);
	}

	if (RowNameStr.EndsWith("?")) {
		const int32 Idx = FMath::Clamp(FMath::RoundToInt32(CondRes), 0, DiagNum-1); // don't overcomplicate, just clamp.
		const FName DiagRow = Seq.DiagRows[Idx]; // this is safe because of the DiagNum<1 above and the above clamp
		UE_LOG(LogDiags, Log, TEXT("%hs: Chosen. Dlg=%i DlgRow=%s Row=%s Condition=%s"),
			__func__, Idx, *DiagRow.ToString(), *RowName.ToString(), *Seq.Condition);
		return AddId(DiagRow);
	}

	if (!CondOk) {
		UE_LOG(LogDiags, Log, TEXT("%hs: Condition not met. condition=%s"), __func__, *Seq.Condition);
		return false; // would allow to add a dialog with the same id, by design, but don't rely on it.
	}

	// add random or regular accordingly. if it ends with * it's ALWAYS random
	if (RowNameStr.EndsWith("*"))
		return AddRnd(Seq);

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

void UDiags::Init() {}

void UDiags::DeInit() {
	Diags = nullptr;
	Chars = nullptr;
	Seqs = nullptr;
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

bool UDiags::CheckCondition(const FString& Expression, float& Res) const {
	Res = 0;
	FString Eval = Expression.TrimStartAndEnd();
	if (LIKELY(Eval.IsEmpty())) return true;

	// "{xx}"	start=0, end=3, len=2, sub={xx}, name =xx
	// "{}"		start=0, end=1, len=0, sub={}, name=""

	int32 PStart = 0;
	int32 PEnd = 0;
	int32 Len = 0;
	FString Sub;
	FString VarName;
	float VarVal = 0;
	// i know some ppl don't like goto-s, but they are fine if you know what you do.
	// a while will incur in an extra unnecessary condition check, which the compiler might or might not optimize.
	replace_next:
		PStart = Eval.Find("{");
		if (PStart<0) goto replace_done; //break; // done

		PEnd = Eval.Find("}", ESearchCase::IgnoreCase, ESearchDir::FromStart, PStart);
		if (UNLIKELY(PEnd <= PStart)) { // this is redundant with below, but i want to have good logs.
			UE_LOG(LogDiags, Warning, TEXT("%hs: Erroneous expression. Missing '}'. Exp='%s'"), __func__, *Eval);
			return false;
		}

		Len = PEnd - PStart -1; // PEnd is at BEFORE the character. so it does not contain it! (hence -1)
		Sub = Eval.Mid(PStart, Len+2);
		VarName = Sub.Mid(1, Len);
		VarName.TrimStartAndEndInline(); // in case the user enters { myvarnamelol }
		if (UNLIKELY(VarName.IsEmpty())) {
			UE_LOG(LogDiags, Warning,
				TEXT("%hs: Erroneous expression: Variable name is empty. need something inside {}."), __func__, *Eval);
			return false;
		}

		const bool Bound = OnGetFlag.IsBound();
		VarVal = LIKELY(Bound) ? OnGetFlag.Execute(FName(VarName)) : 0; // NEEEDS to check for isbound or risk a crash :')
		UE_CLOG(!Bound, LogDiags, Warning, TEXT("%hs: OnGetFlag is not bound! All flags are going to be 0. LOL."), __func__);

		Eval.ReplaceInline(*Sub,*FString::SanitizeFloat(VarVal,0));
		goto replace_next;
	replace_done:
	
	Res = UJUtilsMisc::MathEvaluate(Eval);
	const bool Ok = Res>0 && !FMath::IsNearlyZero(Res);

	UE_LOG(LogDiags, Log,
		TEXT("%hs: Result=%.4f Ok=%i Eval=%s Exp=%s"),
		__func__, Res, Ok, *Eval, *Expression);

	return Ok;
}
