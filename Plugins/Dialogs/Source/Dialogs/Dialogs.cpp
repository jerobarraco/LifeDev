// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "Dialogs.h"

#pragma optimize("", off)
void UDialogs::Add(const FDialog& Diag) {
	Pending.Add(Diag);
	ShowNext();
}

void UDialogs::AddSeq(const TArray<FDialog>& Seq) {
	Pending.Append(Seq);	
	ShowNext();
}

void UDialogs::DiagDone() {
	IsShowing = false;
	ShowNext();
}

void UDialogs::Load(UDataTable* AllDialogs, UDataTable* Chars) {
	AllDiags = IsValid(AllDialogs)? AllDialogs : nullptr;
	AllChars = IsValid(Chars)? Chars: nullptr;
}

void UDialogs::UnLoad() {
	AllDiags = nullptr;
	AllChars = nullptr;
}

bool UDialogs::GetDiag(const FName& RowName, FDialog& OutRow, FDialogChar& OutChar) const {
	if (!IsValid(AllDiags)) return false;
	const FDialog* const Row = AllDiags->FindRow<FDialog>(RowName, TEXT(""));
	if (!Row) return false;

	OutRow = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
	return GetChar(OutRow.CharRow, OutChar);
}

bool UDialogs::GetChar(const FName& RowName, FDialogChar& OutChar) const {
	if (!IsValid(AllChars)) return false;
	const FDialogChar* const Row = AllChars->FindRow<FDialogChar>(RowName, TEXT(""));
	if (!Row) return false;

	OutChar = *Row; // here im copying, which s-u-x. but blueprints wont take a pointer.
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
	Pending.RemoveAt(0); // im sorry mama, i never meant to hurt you.~

	OnShow.Broadcast(Diag);
}

void UDialogs::Stop() {
	if (!IsShowing) return;
	IsShowing = false;

	OnHide.Broadcast();
}
#pragma optimize("", on)
