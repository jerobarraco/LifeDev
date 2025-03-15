// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSeq.h"

EBDoRes UBSeq::DoSelf_Implementation(const float DT) {
	UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return EBDoRes::ABORT; // anomaly

	const EBDoRes R = C->Do(DT);
	if (R == EBDoRes::ABORT) return EBDoRes::ABORT; // bubble up

	if (R == EBDoRes::STOP) {
		StartChild(CurChildI+1);
		if (CurChildI<0)
			return EBDoRes::STOP;
	}

	return EBDoRes::CONTINUE; // should be == Res but meh.
}

bool UBSeq::Plan_Implementation() {
	if (Children.Num() <1) return false;

	// a sequence is valid only of all children are valid.
	// even the ones that will become skipped.
	CostPlanned = 0;
	for (UBBase* const C: Children) {
		if (UNLIKELY(!C)) continue;

		if (!C->Plan()) return false;
		CostPlanned += C->CostPlan();
	}

	CurChildI = 0;
	return true;
}

void UBSeq::SetState_Implementation(const EBState New) {
	const bool WasStopped = State == EBState::STOPPED;
	Super::SetState_Implementation(New);

	if (New == EBState::STARTED && WasStopped) {
		CurChildI = 0;
		StartChild(CurChildI);
	}
}
