// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BPick.h"

EBDoRes UBPick::DoSelf_Implementation(const float DT) {
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
	CostPlanned = FLT_MAX;
	
	// a sequence is valid only of all children are valid.
	// even the ones that will become skipped.
	for (int32 i=0; i<Children.Num(); ++i){
		UBBase* const C= Children[i];
		if (!C) continue;
		if (!C->Plan()) continue;
		const float Cost = C->CostPlan();
		if (Cost < CostPlanned) {
			CostPlanned = Cost;
			CurChildI = i;
		}
	}

	return IsValid(GetCurChild());
}
