// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "Diags.h"

#include "Eval.h"
#include "JUtils/Misc/JUtilsMisc.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiags, Log, Log);

UDiags* UDiags::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	
	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	UDiags* const D = W->GetSubsystem<UDiags>();
	return UNLIKELY(IsValid(D)) ? D : nullptr;
}

void UDiags::AddDiag(const FDiag& Diag) {
	Pending.Add(Diag);
	ShowNext();
}

bool UDiags::AddDiagId(const FName& Row, const bool Warn) {
	UE_LOG(LogDiags, Log, TEXT("%hs: row=%s, warn=%i"), __func__, *Row.ToString(), Warn);
	FDiag OutDialog; FDiagChar OutChar;
	const bool Ok = GetDiag(Row, OutDialog, OutChar, Warn);
	if (UNLIKELY(!Ok)) return false;

	double Res = 0;
	const bool Cond = CheckCondition(OutDialog.Condition, Res);
	if (UNLIKELY(!Cond)) {
		UE_LOG(LogDiags, Log, TEXT("%hs: Condition not met. row=%s condition=%s"),
			__func__, *Row.ToString(), *OutDialog.Condition);
		return false;
	}

	OnAdd.Broadcast(Row, OutDialog); // before addDiag since it will trigger all sorts of other stuff.
	AddDiag(OutDialog);
	return true;
}

bool UDiags::AddId(const FName& Row, const bool Warn) {
	UE_CLOG(Warn, LogDiags, Log, TEXT("%hs: id='%s'"), __func__, *Row.ToString());
	if (UNLIKELY(Row.IsNone())) return false;

	// attempt to add a sequence (can be random) (could trigger another call to AddId)
	if (AddGroupId(Row, false)) return true;

	// otherwise attempt a dialog
	if (LIKELY(AddDiagId(Row, false))) return true;

	// this also would capture a sequence that is empty or the ids are none.
	UE_CLOG(Warn, LogDiags, Warning,
		TEXT("%hs: Could not find dialog nor group with the id=%s. "
			"Or the group was empty or invalid. Or it didn't pass it's condition."), __func__, *Row.ToString());
	return false;
}

bool UDiags::AddIdMany(const TArray<FName>& Rows) {
	const int32 Num = Rows.Num();
	bool Success = false; // return false if nothing was added
	for (int32 i = 0; i < Num; ++i) {
		// done this way on purpose, so if a sequence adds another sequence
		// it will get "expanded" as a parenthesis.
		// AddId is safe to call many times since ShowNext has a flag
		const FName& Row = Rows[i];
		// notice this is recursive. that's on purpose, but be careful.
		const bool Ok = AddId(Row);

		// Success = Success && Ok; // ok will set it to false (faster than branch)
		// now that we have conditions, some dialogs might fail, and we'll be fine with that.
		Success = Success || Ok;
	}

	return Success;
}

bool UDiags::AddGroup(const FDiagGroup& Seq) {
	const TArray<FName>& Rows = Seq.DiagRows;
	const int32 Num = Rows.Num();
	if (Num <= 0) return false;

	double Res;
	const bool CondOk = CheckCondition(Seq.Condition, Res);

	// these two use the condition to select the dialog
	if (Seq.Type == EGroupType::PICK_LOOP) {
		const int32 i = FMath::Max(0, FMath::RoundToZero(FMath::Modulo(Res, Num)));
		const FName DiagRow = Rows[i];
		return AddId(DiagRow);
	}
	
	if (Seq.Type == EGroupType::PICK_CLAMP) {
		const int32 i = FMath::Clamp(Res, 0, Num-1);
		const FName DiagRow = Rows[i];
		return AddId(DiagRow);
	}

	// the rest use the condition to pass
	if (UNLIKELY(!CondOk)) return false;
	
	if (Seq.Type == EGroupType::SEQUENCE) {
		return AddIdMany(Rows);
	}
	
	if (Seq.Type == EGroupType::MATCH) {
		if (UNLIKELY(!CondOk)) return false;
		for (const FName& N: Rows) {
			if (AddId(N)) return true;
		}
		return false;
	}

	if (Seq.Type == EGroupType::RANDOM) {
		TArray<FName> Shuffled = Rows; // make a copy
		UJUtilsMisc::ArrayShuffle<FName>(Shuffled);
		for (const FName& N: Shuffled) {
			if (AddId(N)) return true;
		}
		return false;
	}
	
	return false;
}

bool UDiags::AddGroupId(const FName& RowName, const bool Warn) {
	FDiagGroup Seq;
	const bool Ok = GetGroup(RowName, Seq, Warn);
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

	return AddGroup(Seq);
}

void UDiags::DiagDone() {
	IsShowing = false;
	ShowNext();
}

void UDiags::SetData(
UDataTable* const AllDiags, UDataTable* const AllChars, UDataTable* const AllGroups) {
	Diags = IsValid(AllDiags)? AllDiags : nullptr;
	Chars = IsValid(AllChars)? AllChars: nullptr;
	Groups = IsValid(AllGroups)? AllGroups: nullptr;
}

void UDiags::Init() {
	Eval = UEval::Instance(this);
}

void UDiags::DeInit() {
	Diags = nullptr;
	Chars = nullptr;
	Groups = nullptr;
	Eval = nullptr;
}

bool UDiags::GetDiag(
const FName& RowName, FDiag& OutRow, FDiagChar& OutChar, const bool Warn) const {
	if (UNLIKELY(RowName.IsNone())) return false;
	if (UNLIKELY(!IsValid(Diags))) return false;

	const FDiag* const Row = Diags->FindRow<FDiag>(RowName, TEXT(""), Warn);
	if (UNLIKELY(!Row)) {
		UE_LOG(LogDiags, Verbose, TEXT("Could not find dialog for row=%s"), *RowName.ToString());
		return false;
	}

	OutRow = *Row; // here im copying, which s-u-x. but blueprints won't take a pointer.
	GetChar(OutRow.CharRow, OutChar); // ignore if the char is not found for the result, we only care about Diags
	return true;
}

bool UDiags::GetChar(const FName& RowName, FDiagChar& OutChar, const bool Warn) const {
	if (UNLIKELY(RowName.IsNone())) return false;
	if (UNLIKELY(!IsValid(Chars))) return false;

	const FDiagChar* const Row = Chars->FindRow<FDiagChar>(RowName, TEXT(""), Warn);
	if (UNLIKELY(!Row)) {
		UE_LOG(LogDiags, Warning, TEXT("Could not find character for row=%s"), *RowName.ToString());
		return false;
	}

	OutChar = *Row; // here im copying, which s-u-x. but blueprints won't take a pointer.
	return true;
}

bool UDiags::GetGroup(const FName& RowName, FDiagGroup& OutGroup, const bool Warn) const {
	if (UNLIKELY(RowName.IsNone())) return false;
	if (UNLIKELY(!IsValid(Groups))) return false;

	const FDiagGroup* const Row =
		Groups->FindRow<FDiagGroup>(RowName, TEXT(""), Warn);
	if (UNLIKELY(!Row)) {
		UE_LOG(LogDiags, Verbose, TEXT("Could not find sequence for row=%s"), *RowName.ToString());
		return false;
	}

	OutGroup = *Row; // here im copying, which s-u-x. but blueprints won't take a pointer. also it's safer.
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
	FDiag Diag = Pending[0];
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
	// empty expressions passes (true)
	// not unlikely because i don't know how many dialogs have an empty condition
	if (Expression.TrimStartAndEnd().IsEmpty()) {
		Res = 1; // avoid issues down the road
		return true;
	}

	if (UNLIKELY(!Eval)) {
		UE_LOG(LogDiags, Warning, TEXT("%hs Could not obtain Eval subsystem. Make sure it's enabled in config."), __func__);
		return false;
	}

	const bool Ok = Eval->Eval(Expression, Res);
	return Ok && Res > 0;
}
