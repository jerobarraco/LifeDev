// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSeq.h"

EBDoRes UBSeq::Do_Implementation(const float DT) {
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
	if (!CanDoSelf()) return false;

	// a sequence is valid only of all children are valid.
	// even the ones that will become skipped.
	for (UBBase* const C: Children) {
		if (!C) continue;
		if (!C->Plan()) return false;
	}

	return true;
}

bool UBSeq::CanDoSelf_Implementation() const {
	return Children.Num() > 0;
}

float UBSeq::CostPlan_Implementation() const {
	float S=0;
	for (const UBBase* const C: Children) {
		S+= C->CostPlan();
	}

	return S;
}
