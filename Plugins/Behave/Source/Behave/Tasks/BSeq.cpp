// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSeq.h"

#include <UObject/GCObjectScopeGuard.h>

EBDoRes UBSeq::DoSelf_Implementation(const float DT) {
	const EBDoRes R = Super::DoSelf_Implementation(DT);
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
	CostCur = 0;
	for (UBBase* const C: Children) {
		if (UNLIKELY(!C)) continue;

		FGCObjectScopeGuard CreatedObjectGuard(C);
		if (!C->Plan()) return false;
		CostCur += C->Cost();
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
	} else if (New == EBState::STOPPED) {
		for (UBBase* C: Children)
			if (LIKELY(C)) C->SetState(EBState::STOPPED);
	}
}
