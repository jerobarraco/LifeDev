// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BFirst.h"

EBDoRes UBFirst::DoSelf_Implementation(const float DT) {
	UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return EBDoRes::ABORT; // anomaly

	const EBDoRes R = C->Do(DT);
	return R;
}

bool UBFirst::Plan_Implementation() {
	if (UNLIKELY(Children.Num()<1)) return false;

	CurChildI = -1;
	CostPlanned = FLT_MAX;

	// pick the first
	for (int32 i=0; i<Children.Num(); ++i){
		UBBase* const C= Children[i];
		if (UNLIKELY(!C)) continue;

		if (!C->Plan()) continue;
		CostPlanned = C->CostPlan();
		CurChildI = i;
	}

	return IsValid(GetCurChild());
}

void UBFirst::SetState_Implementation(const EBState New) {
	// const bool WasStopped = State == EBState::STOPPED;
	Super::SetState_Implementation(New);

	// unplanned will have childI <0. StartChild can handle that.
	// if (New == EBState::STARTED && WasStopped)
		// StartChild(CurChildI);
	SetCurChildSate(New);
}
