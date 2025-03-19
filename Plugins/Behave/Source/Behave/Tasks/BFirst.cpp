// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BFirst.h"

#include <UObject/GCObjectScopeGuard.h>

bool UBFirst::Plan_Implementation() {
	if (UNLIKELY(Children.Num()<1)) return false;

	CurChildI = -1;
	CostCur = FLT_MAX;

	// pick the first
	for (int32 i=0; i<Children.Num(); ++i){
		UBBase* const C= Children[i];
		FGCObjectScopeGuard CreatedObjectGuard(C);
		if (UNLIKELY(!C)) continue;
		
		if (!C->Plan()) continue;

		CostCur = C->Cost();
		CurChildI = i;
		break;
	}

	return IsValid(GetCurChild());
}

void UBFirst::SetState_Implementation(const EBState New) {
	Super::SetState_Implementation(New);
	SetCurChildSate(New);
}

float UBFirst::Cost_Implementation() {
	if (State == EBState::STARTED) {
		CostPlanFromCurChild();
	}

	return CostCur;
}
