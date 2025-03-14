// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BPick.h"

EBDoRes UBPick::Do_Implementation(const float DT) {
	UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return EBDoRes::ABORT; // anomaly

	const EBDoRes R = C->Do(DT);
	if (R == EBDoRes::ABORT) return EBDoRes::ABORT; // bubble up
	if (R == EBDoRes::STOP) return EBDoRes::STOP;
	return EBDoRes::CONTINUE; // should be == Res but meh.
}

bool UBPick::Plan_Implementation() {
	if (UNLIKELY(Children.Num()<1)) return false;

	CurChildI = -1;
	float Min = FLT_MAX;
	
	// a sequence is valid only of all children are valid.
	// even the ones that will become skipped.
	for (int32 i=0; i<Children.Num(); ++i){
		UBBase* const C= Children[i];
		if (!C) continue;
		if (!C->Plan()) continue;
		const float Cost = C->CostPlan();
		if (Cost < Min) {
			Min = Cost;
			CurChildI = i;
		}
	}

	return IsValid(GetCurChild());
}

float UBPick::CostPlan_Implementation() const {
	// This is a bit of a problem. since the cost is only known after planning
	const UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return 0; // anomaly

	return C->CostPlan();
}
