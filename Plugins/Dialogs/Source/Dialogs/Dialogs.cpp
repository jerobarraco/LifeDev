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

	OnStop.Broadcast();
}
#pragma optimize("", on)
