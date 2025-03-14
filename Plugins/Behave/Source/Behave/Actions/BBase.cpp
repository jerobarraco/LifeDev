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
	UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return EBDoRes::ABORT; // anomaly

	const EBDoRes R = C->Do(DT);
	if (R == EBDoRes::ABORT) return EBDoRes::ABORT; // bubble up

	// This base object is more abstract than anything else. so i will just return the child.
	// which is kind of the best thing, specially for classes that inherit from this
	return R;
}

void UBBase::SetState_Implementation(const EBState New) {
	State = New;
	if (State == EBState::STOPPED) StopCurChild();
	OnState.Broadcast(State);
}

UBBase* UBBase::GetCurChild() {
	const int32 Num = Children.Num();
	if (UNLIKELY(CurChildI<0 || CurChildI>=Num)) return nullptr;
	return Children[CurChildI];
}

void UBBase::StopCurChild() {
	SetCurChildSate(EBState::STOPPED);
}

void UBBase::SetCurChildSate(const EBState New) {
	// i could call GetCurChild here. but it's one more if. NOOooooooooouuuu!
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
