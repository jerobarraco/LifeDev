// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Actions/BBase.h"
#include "UObject/GCObjectScopeGuard.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);

UCBehave::UCBehave():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetTickableWhenPaused(false);
	SetComponentTickEnabled(true);

	// static ConstructorHelpers::FObjectFinder<UDataTable>
		// CADT(TEXT("/Behave/Test/TestActions_DT.TestActions_DT"));
	// TasksDT = CADT.Object;
}

void UCBehave::TickComponent(const float DeltaTime, const enum ELevelTick TickType,
FActorComponentTickFunction* const ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (UNLIKELY(!TaskCur)) {
		SetComponentTickEnabled(false);
		Plan();
		return;
	}

	const EBDoRes Res = TaskCur->DoSelf(DeltaTime);
	if (LIKELY(Res == EBDoRes::CONTINUE)) return;
	
	if (Res == EBDoRes::STOP || Res == EBDoRes::ABORT) {
		CurStop();  // will plan next tick
	}
}

void UCBehave::BeginPlay() {
	Super::BeginPlay();
	TasksDT.LoadSynchronous();
	for (const FName& N: TasksToLoad) {
		TaskAdd(TaskLoad(N));
	}
}

void UCBehave::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	for (UBBase* const C: Tasks) {
		if (LIKELY(C)) C->DeInit();
	}

	// TODO should have deinit on removing an action.
	// but i'd need to make actions protected.
	// and i'd need to have a bunch of extra functions for that
	Tasks.Empty(0); // uobjects can't be directly destroyed.
	Super::EndPlay(EndPlayReason);
}

void UCBehave::CurStop() {
	if (UNLIKELY(!TaskCur)) return;

	TaskCur->SetState(EBState::STOPPED);
	TaskCur = nullptr;
}

void UCBehave::TaskRegister(UBBase* const Action) {
	// add "unique" is important
	Action->OnState.AddUniqueDynamic(this, &UCBehave::TaskStateUp);
}

UBBase* UCBehave::TaskNew(const TSubclassOf<UBBase>& Class) {
	UBBase* const B = NewObject<UBBase>(this, Class.Get());
	if (B) B->Init(this); // force register on any new class
	// notes on why register is like this on b->register
	return B;
}

UBBase* UCBehave::TaskLoad(const FName Row) {
	UE_LOG(LogCBehave, Log, TEXT("%hs Row=%s"), __func__, *Row.ToString());

	const UDataTable* const DT = TasksDT.Get();
	if (UNLIKELY(!DT)) {
		UE_LOG(LogCBehave, Warning, TEXT("%hs datatable not loaded. Stop."), __func__);
		return nullptr;
	}

	FBAction* const ActDef = DT->FindRow<FBAction>(Row, "");
	if (UNLIKELY(!ActDef)) {
		UE_LOG(LogCBehave, Warning, TEXT("%hs could not find action id=%s"), __func__, *Row.ToString());
		return nullptr;
	}

	UBBase* const Action = TaskNew(ActDef->Class);
	if (UNLIKELY(!Action)) {
		UE_LOG(LogCBehave, Warning, TEXT("%hs could not create action id=%s class=%s"),
			__func__, *Row.ToString(), *ActDef->Class.Get()->GetName());
		return nullptr;
	}
	
	Action->ID = Row;
	for (const FName& C: ActDef->Children) {
		UBBase* const Child = TaskLoad(C);
		if (UNLIKELY(!Child)) continue; // load already warns
		Action->Children.Add(Child);
	}

	return Action;
}

void UCBehave::TaskAdd(UBBase* const Task, const int32 Priority) {
	if (UNLIKELY(!IsValid(Task))) return;

	// it's a side effect but the way the code is set. init is only called after the action and all its children are loaded. which is nice.
	Task->Init(this);
	// the code suggest it will crash if Priority is <0 or >Num
	const int32 Index = Priority <0 ? Tasks.Num() : FMath::Min(Priority, Tasks.Num());
	Tasks.Insert(Task, Index);
}

int32 UCBehave::TaskRem(const FName Row) {
	const int32 Num = Tasks.Num();
	for (int32 i =0; i<Num; ++i) {
		UBBase* const A = Tasks[i];
		if (UNLIKELY(!IsValid(A))) continue;
		
		if (LIKELY(A->ID != Row)) continue;
		
		A->DeInit();
		Tasks.RemoveAt(i);
		return i;
	}

	return -1;
}


void UCBehave::PlanDo() {
	TaskPlan = nullptr;
	// FCriticalSection this is not what i want here.
	
	// If this looks simple is because it is. a lot of the planning is offloaded to the actions themselves.
	// Unfortunately this does not allow to perform a A* search. but i'd refactor that (very) later on.

	// run through actions. ask if possible.
	// they are sorted by priority.
	// we don't care about cost at this point. the action itself cares.
	for (UBBase* const A: Tasks) {
		// might make it slower but maybe safer
		FGCObjectScopeGuard CreatedObjectGuard(A); // TODO add this to the BBase using children
		if (UNLIKELY(!A)) continue;

		if (!A->Plan()) continue;

		TaskPlan = A;
		break;
	}

	AsyncTask(ENamedThreads::GameThread, [this]{
		PlanDone();
	});
}

void UCBehave::Plan() {
	if (UNLIKELY(IsPlanning)) return;
	IsPlanning = true;
	Async(EAsyncExecution::Thread, [this]{
		PlanDo();
	});
}

void UCBehave::PlanDone() {
	const bool TaskChanged = TaskCur != TaskPlan;
	const bool HasTask = !!TaskPlan;
	// avoid stopping old if unnecessary. (it might be the same as before)
	if (UNLIKELY(TaskChanged))
		CurStop();
	
	// do this after stop. since curstop will clean taskCur.
	TaskCur = TaskPlan; // could be null. in that case it's fine, it clears the taskCur.
	TaskPlan = nullptr;

	// start new task.
	if (LIKELY(TaskChanged && HasTask)) {
		TaskCur->SetState(EBState::STARTED);
		SetComponentTickEnabled(true);
	}

	if (!HasTask || UsePlanLoop) {
		// retry later
		FTimerHandle H;
		const UWorld* const World = GetWorld();
		if (LIKELY(World))
			World->GetTimerManager().SetTimer(H, this, &UCBehave::Plan, PlanWaitTime);
	}

	IsPlanning = false;
}

void UCBehave::TaskStateUp(UBBase* const Act, const EBState State) {
	UE_LOG(LogCBehave, Log, TEXT("%hs State=%s Act=%s"),
		__func__, *UEnum::GetValueAsString(State), *GetNameSafe(Act));

	OnState.Broadcast(Act, State);
	if (State == EBState::STOPPED) {
		TaskLeafCur = nullptr;
	} else if (State == EBState::STARTED && LIKELY(IsValid(Act))) {
		TaskLeafCur = Act;
	}
}
