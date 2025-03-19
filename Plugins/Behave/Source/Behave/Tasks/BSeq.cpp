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
	Super::SetState_Implementation(New);

	if (State == EBState::STOPPED) {
		for (UBBase* C: Children)
			if (LIKELY(C)) C->SetState(EBState::STOPPED);
		CurChildI = 0; // restart. or should i set to -1 to force a re-plan?
	} else if (State == EBState::STARTED) {
		StartChild(CurChildI);
	}
}

void UBSeq::CostCalc_Implementation() {
	if (State != EBState::STARTED) return; // no need to recalculate anything.

	if (Children.Num()<1 || CurChildI <0) {
		CostCur = FLT_MAX;
		return;
	}

	CostCur = 0;
	for (int32 i=CurChildI; i<Children.Num(); ++i) {
		UBBase* const C = Children[i];
		FGCObjectScopeGuard CreatedObjectGuard(C);
		if (UNLIKELY(!C)) continue;
		
		CostCur += C->Cost();
	}
}
