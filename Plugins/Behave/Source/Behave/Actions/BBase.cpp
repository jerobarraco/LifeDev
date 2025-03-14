// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

bool UBBase::CanDoSelf_Implementation() const {
	return true;
}

bool UBBase::CanDo_Implementation() const {
	return CanDoSelf();
}

EBDoRes UBBase::Do_Implementation(const float DT) {
	return EBDoRes::STOP;
}

void UBBase::SetState_Implementation(const EBState New) {
	State = New;
	if (State == EBState::STOPPED) StopCurChild();
	OnState.Broadcast(State);
}

void UBBase::StopCurChild() {
	SetCurChildSate(EBState::STOPPED);
}

void UBBase::SetCurChildSate(const EBState New) {
	const int32 Num = Children.Num();
	if (UNLIKELY(CurChildI<0 || CurChildI>=Num)) return;
	Children[CurChildI]->SetState(State);
}

void UBBase::StartChild(const int32 I) {
	StopCurChild();

	if (I<0 || I>= Children.Num()) {
		CurChildI = -1;
		return;
	}

	CurChildI = I;
	SetCurChildSate(EBState::STARTED);
}
