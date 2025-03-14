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

	Chosen = nullptr;
	float Min = FLT_MAX;
	
	// a sequence is valid only of all children are valid.
	// even the ones that will become skipped.
	for (UBBase* const C: Children) {
		if (!C) continue;
		if (!C->Plan()) continue;
		const float Cost = C->CostPlan();
		if (Cost < Min) {
			Min = Cost;
			Chosen = C;
		}
	}

	return IsValid(Chosen);
}

bool UBPick::CanDoSelf_Implementation() const {
	return Children.Num()>0;
}

float UBPick::CostPlan_Implementation() const {
	// This is a bit of a problem. since the cost is only known after planning
	if (!Chosen) return 0;
	return Chosen->CostPlan();
}
