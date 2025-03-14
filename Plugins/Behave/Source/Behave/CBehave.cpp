// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Actions/BBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);

#pragma optimize("", off)
UCBehave::UCBehave():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetTickableWhenPaused(false);
	SetComponentTickEnabled(true);
}

void UCBehave::TickComponent(const float DeltaTime, const enum ELevelTick TickType,
	FActorComponentTickFunction* const ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (UNLIKELY(!ActionCur)) {
		PlanStart();
		SetComponentTickEnabled(false);
		return;
	}

	ActionCur->Do(DeltaTime);
}

void UCBehave::PlanDo() {
	Planned = nullptr;

	// If this looks simple is because it is. a lot of the planning is offloaded to the actions themselves.
	// Unfortunately this does not allow to perform a A* search. but i'd refactor that (very) later on.

	// run through actions. ask if possible.
	// they are sorted by priority.
	// we don't care about cost at this point. the action itself cares.
	for (UBBase* const A: Actions) {
		if (UNLIKELY(!A)) continue;
		if (!A->CanDo()) continue;
		Planned = A;
		break;
	}

	AsyncTask(ENamedThreads::GameThread, [this]{
		PlanDone();
	});
}

void UCBehave::PlanStart() {
	if (UNLIKELY(IsPlanning)) return;
	IsPlanning = true;
	Async(EAsyncExecution::Thread, [this]{
		PlanDo();
	});
}

void UCBehave::PlanDone() {
	if (LIKELY(ActionCur))
		ActionCur->SetState(0);

	ActionCur = Planned; // could be null. in that case it remains clear.
	if (LIKELY(ActionCur))
		ActionCur->SetState(1);

	IsPlanning = false;
}

#pragma optimize("", on)
