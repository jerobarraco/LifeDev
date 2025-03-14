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
	// TODO Stop current action
	if (LIKELY(ActionCur)) {
		ActionCur->SetState(0);
		ActionCur = nullptr;
	}
	if (Plan.Num()>0) {
		ActionCur = Plan[0];
	}
	ActionCur->SetState(1);
	IsPlanning = false;
}

#pragma optimize("", on)
