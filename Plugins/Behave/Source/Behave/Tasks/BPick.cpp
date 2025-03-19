// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BPick.h"

#include <UObject/GCObjectScopeGuard.h>

bool UBPick::Plan_Implementation() {
	if (UNLIKELY(Children.Num()<1)) return false;

	CurChildI = -1;
	CostPlanned = FLT_MAX;
	
	// a sequence is valid only of all children are valid.
	// even the ones that will become skipped.
	for (int32 i=0; i<Children.Num(); ++i){
		UBBase* const C= Children[i];
		if (UNLIKELY(!C)) continue;

		FGCObjectScopeGuard CreatedObjectGuard(C);
		if (!C->Plan()) continue;

		const float Cost = C->CostPlan();
		if (Cost < CostPlanned) {
			CostPlanned = Cost;
			CurChildI = i;
		}
	}

	return IsValid(GetCurChild());
}

void UBPick::SetState_Implementation(const EBState New) {
	// const bool WasStopped = State == EBState::STOPPED;
	Super::SetState_Implementation(New);

	SetCurChildSate(New);

	// unplanned will have childI <0. StartChild can handle that.
	// if (New == EBState::STARTED && WasStopped)
		// StartChild(CurChildI);
}

float UBPick::CostPlan_Implementation() {
	if (State == EBState::STARTED) {
		CostPlanFromCurChild();
	}

	return CostPlanned;// Allow for replan
}
