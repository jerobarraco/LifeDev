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

	const EBDoRes Res = ActionCur->Do(DeltaTime);
	if (LIKELY(Res == EBDoRes::CONTINUE)) return;
	
	if (Res == EBDoRes::STOP || Res == EBDoRes::ABORT) {
		CurStop();  // will plan next tick
	}
}

void UCBehave::BeginPlay() {
	Super::BeginPlay();

	for (const TSubclassOf<UBBase>& C: ActionClasses){
		UBBase* const B = NewObject<UBBase>(this, C.Get());
		if (UNLIKELY(!IsValid(B))) {
			UE_LOG(LogCBehave, Warning, TEXT("Behave:%hs falied to create obj for class=%s"),
				__func__, *C.Get()->GetName());
			continue;
		}
		B->Register(this);
		Actions.Add(B);
	}
}

void UCBehave::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Actions.Empty(); // uobjects can't be directly destroyed.
}

void UCBehave::CurStop() {
	if (UNLIKELY(!ActionCur)) return;

	ActionCur->SetState(EBState::STOPPED);
	ActionCur = nullptr;
}

void UCBehave::Register(UBBase* const Action) {
	Action->OnState.AddUniqueDynamic(this, &UCBehave::ActStateUp);
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
		if (!A->Plan()) continue;
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
	CurStop();

	ActionCur = Planned; // could be null. in that case it remains clear.
	if (LIKELY(ActionCur)) {
		ActionCur->SetState(EBState::STARTED);
		SetComponentTickEnabled(true);
	}

	IsPlanning = false;
}

void UCBehave::ActStateUp(UBBase* const Act, const EBState State) {
	UE_LOG(LogCBehave, Log, TEXT("%hs State=%s Act=%s"),
		__func__, *UEnum::GetValueAsString(State), *GetNameSafe(Act));

	OnState.Broadcast(Act, State);
	if (State == EBState::STOPPED) {
		ActionChildCur = nullptr;
	} else if (State == EBState::STARTED && Act) {
		ActionChildCur = Act;
	}
}

#pragma optimize("", on)
