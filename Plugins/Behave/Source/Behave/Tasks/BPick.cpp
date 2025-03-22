// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BPick.h"

#include <UObject/GCObjectScopeGuard.h>

bool UBPick::Plan_Implementation() {
	if (UNLIKELY(Children.Num()<1)) return false;

	CurChildI = -1;
	CostCur = FLT_MAX;

	// TAtomic is deprecated
	std::atomic<int32> AChild = -1;
	std::atomic<int32> ACost = FLT_MAX;

	FCriticalSection Mutex;

	// start by planning all branches
	// specifying & to capture mutex, the atomics, and this.
	ParallelFor(Children.Num(), [&](const int32 i) {
		UBBase* const C= Children[i];
		if (UNLIKELY(!C)) return;

		FGCObjectScopeGuard CreatedObjectGuard(C);
		if (!C->Plan()) return;
		const float Cost = C->Cost();
		if (Cost < ACost.load()) {
			FScopeLock Lock(&Mutex);// lock because we're going to change 2 vars
			ACost = Cost;
			AChild = i;
		}
	});

	CurChildI = AChild.load();
	CostCur = ACost.load();
	
	// now gather the cost. can't do this on the parallel for due to race conditions.
	// for (int32 i=0; i<Children.Num(); ++i){
		// UBBase* const C= Children[i];
		// if (UNLIKELY(!C)) continue;

		// FGCObjectScopeGuard CreatedObjectGuard(C);
		// const float Cost = C->Cost();
		// if (Cost < CostCur) {
			// CostCur = Cost;
			// CurChildI = i;
		// }
	// }

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

// https://georgy.dev/posts/parallel-for-loop/
// https://m.youtube.com/watch?v=XJMyNM8xmS0
