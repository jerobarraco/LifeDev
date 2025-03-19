// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BWait.h"

bool UBWait::Plan_Implementation() {
	TimeRem = TimeWait;
	CostCalc();
	return true;
}

EBDoRes UBWait::DoSelf_Implementation(const float DT) {
	TimeRem -= DT;
	if (UNLIKELY(TimeRem<=0 || FMath::IsNearlyZero(TimeRem)))
		return EBDoRes::STOP;
	
	return EBDoRes::CONTINUE;
}

void UBWait::SetState_Implementation(const EBState New) {
	Super::SetState_Implementation(New);
	// reset time on start. this is redundant from Plan, but i'm testing things out.
	if (State == EBState::STARTED)
		TimeRem = TimeWait;
}

void UBWait::CostCalc_Implementation() {
	CostCur = TimeWait * CostPerSecond; // easier to recalculate than to check the state (branch)
}
