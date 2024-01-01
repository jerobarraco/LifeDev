// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Diags.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiags, Log, Log);

void UDiags::AddDiag(const FDialog& Diag) {
	Pending.Add(Diag);
	ShowNext();
}

bool UDiags::AddDiagId(const FName& Row) {
	FDialog OutDialog; FDialogChar OutChar;
	const bool Ok = GetDiag(Row, OutDialog, OutChar);
	if (!Ok) return false;

	AddDiag(OutDialog);
	return true;
}

bool UDiags::AddId(const FName& Row) {
	if (Row.IsNone()) return false;

	// attempt to add a sequence (can be random)
	if (AddSeqId(Row)) return true;

	// otherwise attempt a dialog
	if (AddDiagId(Row)) return true;

	UE_LOG(LogDiags, Warning, TEXT("Could not find dialog nor sequence with the id=%s"), *Row.ToString());
	return false;
}

bool UDiags::AddManyIds(const TArray<FName>& Rows) {
	bool Success = true;
	const int32 Num = Rows.Num();
	for (int32 i = 0; i < Num; ++i) {
		// done this way on purpose, so if a sequence adds another sequence
		// it will get "expanded" as a parenthesis.
		// AddId is safe to call many times since ShowNext has a flag
		const FName& Row = Rows[i];
		const bool Ok = AddId(Row);
		// notice this is kind of recursive. that's on purpose but be careful.
		if (!Ok) {
			Success = false;
		}
	}

	return Success;
}

bool UDiags::AddSeq(const FDialogSequence& Seq) {
	const TArray<FName>& Rows = Seq.DiagRows;
	return AddManyIds(Rows);
}

bool UDiags::AddSeqId(const FName& RowName) {
	FDialogSequence Seq;
	const bool Ok = GetSeq(RowName, Seq);
	if (!Ok) return false;

	// add random or regular accordingly. if it ends with * it's ALWAYS random
	if (RowName.ToString().EndsWith("*")) {
		return AddRnd(Seq);
	}

	// since rand is a seq too. if rand doesn't exists the seq doesn't exists.
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

void UDiags::SetData(UDataTable* AllDiags, UDataTable* AllChars, UDataTable* AllSeqs) {
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

bool UDiags::GetDiag(const FName& RowName, FDialog& OutRow, FDialogChar& OutChar) const {
	if (RowName.IsNone()) return false;
	if (!IsValid(Diags)) return false;

	const FDialog* const Row = Diags->FindRow<FDialog>(RowName, TEXT(""));
	if (!Row) {
		UE_LOG(LogDiags, Verbose, TEXT("Could not find dialog for row=%s"), *RowName.ToString());
		return false;
	}

	OutRow = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	GetChar(OutRow.CharRow, OutChar); // ignore if the char is not found for the result, we only care about Diags
	return true;
}

bool UDiags::GetChar(const FName& RowName, FDialogChar& OutChar) const {
	if (RowName.IsNone()) return false;
	if (!IsValid(Chars)) return false;

	const FDialogChar* const Row = Chars->FindRow<FDialogChar>(RowName, TEXT(""));
	if (!Row)  {
		UE_LOG(LogDiags, Warning, TEXT("Could not find character for row=%s"), *RowName.ToString());
		return false;
	}

	OutChar = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	return true;
}

bool UDiags::GetSeq(const FName& RowName, FDialogSequence& OutSeq) const {
	if (RowName.IsNone()) return false;
	if (!IsValid(Seqs)) return false;

	const FDialogSequence* const Row = Seqs->FindRow<FDialogSequence>(RowName, TEXT(""));
	if (!Row) {
		UE_LOG(LogDiags, Verbose, TEXT("Could not find sequence for row=%s"), *RowName.ToString());
		return false;
	}

	OutSeq = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	return true;
}

void UDiags::ShowNext() {
	if (IsShowing) return;
	// this is important for stop to work correctly. and also in general
	IsShowing = true;
	
	if (Pending.IsEmpty()) {
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
	if (!IsShowing) return;
	IsShowing = false;

	OnDone.Broadcast();
}
