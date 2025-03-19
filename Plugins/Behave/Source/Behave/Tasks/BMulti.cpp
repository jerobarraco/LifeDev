// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMulti.h"

#include <UObject/GCObjectScopeGuard.h>

EBDoRes UBMulti::DoSelf_Implementation(const float DT) {
	bool IsAborting = false;
	bool AllDone = true;
	for (UBBase* const C: Children) {
		if (UNLIKELY(!C)) continue;
		if (C->GetState() != EBState::STARTED) continue; // ignore finished ones, AllDone keeps ok.
		
		const EBDoRes R = C->Do(DT);
		if (R == EBDoRes::ABORT)
			IsAborting = true;
		else if (R == EBDoRes::STOP)
			C->SetState(EBState::STOPPED);
		else if (R==EBDoRes::CONTINUE)
			AllDone = false;
	}

	if (UNLIKELY(IsAborting)) return EBDoRes::ABORT;
	if (UNLIKELY(AllDone)) return EBDoRes::STOP;
	return EBDoRes::CONTINUE;
}

bool UBMulti::Plan_Implementation() {
	if (UNLIKELY(Children.Num() <1)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Multi with no children. Will not work. Id=%s"),
			__func__, *ID.ToString());
		return false;
	}
	
	// a Multiuence is valid only of all children are valid.
	// even the ones that will become skipped.
	for (UBBase* const C: Children) {
		if (UNLIKELY(!C)) continue;
		FGCObjectScopeGuard CreatedObjectGuard(C);
		if (!C->Plan()) return false;
	}

	CostPlanCalc();

	CurChildI = 0;
	return true;
}

void UBMulti::SetState_Implementation(const EBState New) {
	// const bool WasStopped = State == EBState::STOPPED;
	Super::SetState_Implementation(New);

	for (UBBase* const C: Children)
		if (LIKELY(C)) C->SetState(New);
	// if (New == EBState::STARTED && WasStopped) {
		// const int32 Num = Children.Num();
		// for (int32 i=0; i<Num; ++i)
			// StartChild(i);
	// }
}

void UBMulti::CostPlanCalc() {
	CostCur = 0;
	// not entirely sure if i should add them or get the bigger...
	for (UBBase* const C: Children) {
		const float CostChild = C->Cost();
		if (CostChild > CostCur)
			CostCur = CostChild; 
	}
}

float UBMulti::Cost_Implementation() {
	if (State == EBState::STARTED) CostPlanCalc();
	return CostCur;
}
