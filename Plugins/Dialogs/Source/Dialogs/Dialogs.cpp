// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "Dialogs.h"

#pragma optimize("", off)
void UDialogs::AddDiag(const FDialog& Diag) {
	Pending.Add(Diag);
	ShowNext();
}

void UDialogs::AddMany(const TArray<FDialog>& InDiags) {
	Pending.Append(InDiags);
	ShowNext();
}

bool UDialogs::AddDiagId(const FName& Row) {
	FDialog OutDialog; FDialogChar OutChar;
	const bool Ok = GetDiag(Row, OutDialog, OutChar);
	if (!Ok) return false;

	AddDiag(OutDialog);
	return true;
}

bool UDialogs::AddId(const FName& Row) {
	if (AddSeqId(Row)) return true;
	return AddDiagId(Row);
}

bool UDialogs::AddSeq(const FDialogSequence& Seq) {
	TArray<FDialog> OutDialogs; TArray<FDialogChar> OutChars;

	const TArray<FName>& Rows = Seq.DiagRows;
	const int32 Num = Rows.Num();
	OutDialogs.Reserve(Num);
	OutChars.Reserve(Num);

	// success is whether they succeeded all at once
	bool Success = true;
	for (int32 i =0; i<Num; ++i) {
		const FName& Row = Rows[i];
		FDialog Diag; FDialogChar Char;
		const bool Ok = GetDiag(Row, Diag, Char);
		if (!Ok) {
			Success = false;
			continue;
		}

		OutDialogs.Add(Diag);
		OutChars.Add(Char);
	}

	AddMany(OutDialogs);
	
	return Success;
}

bool UDialogs::AddSeqId(const FName& RowName) {
	FDialogSequence OutSeq; TArray<FDialog> OutDiags; TArray<FDialogChar> OutChars;
	const bool Ok = GetSeq(RowName, OutSeq);
	if (!Ok) return false;
	
	return AddSeq(OutSeq);
}

bool UDialogs::AddRnd(const FDialogSequence& Seq) {
	int32 Num = Seq.DiagRows.Num();
	if (Num <= 0) return false;
	const int32 i = FMath::RandRange(0, Num);
	return AddId(Seq.DiagRows[i]);
}

bool UDialogs::AddRndId(const FName& RowName) {
	FDialogSequence Seq;
	const bool Ok = GetSeq(RowName, Seq);
	if (!Ok) return false;
	return AddRnd(Seq);
}

void UDialogs::DiagDone() {
	IsShowing = false;
	ShowNext();
}

void UDialogs::SetData(UDataTable* AllDialogs, UDataTable* AllChars, UDataTable* AllSeqs) {
	Diags = IsValid(AllDialogs)? AllDialogs : nullptr;
	Chars = IsValid(AllChars)? AllChars: nullptr;
	Seqs = IsValid(AllSeqs)? AllSeqs: nullptr;
}

void UDialogs::Init() {}

void UDialogs::DeInit() {
	Diags = nullptr;
	Chars = nullptr;
	Seqs = nullptr;
}

bool UDialogs::GetDiag(const FName& RowName, FDialog& OutRow, FDialogChar& OutChar) const {
	if (RowName.IsNone()) return false;
	if (!IsValid(Diags)) return false;

	const FDialog* const Row = Diags->FindRow<FDialog>(RowName, TEXT(""));
	if (!Row) {
		UE_LOG(LogTemp, Warning, TEXT("Could not find dialog for row=%s"), *RowName.ToString());
		return false;
	}

	OutRow = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	GetChar(OutRow.CharRow, OutChar); // ignore if the char is not found for the result, we only care about dialogs
	return true;
}

bool UDialogs::GetChar(const FName& RowName, FDialogChar& OutChar) const {
	if (RowName.IsNone()) return false;
	if (!IsValid(Chars)) return false;

	const FDialogChar* const Row = Chars->FindRow<FDialogChar>(RowName, TEXT(""));
	if (!Row)  {
		UE_LOG(LogTemp, Warning, TEXT("Could not find character for row=%s"), *RowName.ToString());
		return false;
	}

	OutChar = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	return true;
}

bool UDialogs::GetSeq(const FName& RowName, FDialogSequence& OutSeq) const {
	if (RowName.IsNone()) return false;
	if (!IsValid(Seqs)) return false;

	const FDialogSequence* const Row = Seqs->FindRow<FDialogSequence>(RowName, TEXT(""));
	if (!Row) {
		UE_LOG(LogTemp, Warning, TEXT("Could not find sequence for row=%s"), *RowName.ToString());
		return false;
	}

	OutSeq = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	return true;
}

void UDialogs::ShowNext() {
	if (IsShowing) return;
	// this is important for stop to work correctly.
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

void UDialogs::Stop() {
	if (!IsShowing) return;
	IsShowing = false;

	OnDone.Broadcast();
}
#pragma optimize("", on)
