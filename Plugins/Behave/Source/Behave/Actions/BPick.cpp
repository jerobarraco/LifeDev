// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BPick.h"

EBDoRes UBPick::Do_Implementation(const float DT) {
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

bool UBPick::Plan_Implementation() {
	if (!CanDoSelf()) return false;

	float Min = Children[0]->PlanCost();
	int32 Smaller = 0;
	// a sequence is valid only of all children are valid.
	// even the ones that will become skipped.
	for (UBBase* const C: Children) {
		if (!C) continue;
		if (!C->Plan()) return false;
	}

	return true;
}

bool UBPick::CanDoSelf_Implementation() const {
	return Children.Num()>0;
}

float UBPick::PlanCost_Implementation() const {
	// This is a bit of a problem. since the cost is only known after planning
	return Super::PlanCost_Implementation();
}
