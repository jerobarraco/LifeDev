// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Actions/BBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);

UCBehave::UCBehave():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetTickableWhenPaused(false);
	SetComponentTickEnabled(true);

	static ConstructorHelpers::FObjectFinder<UDataTable>
		CADT(TEXT("/Behave/Test/TestActions_DT.TestActions_DT"));
	ActionsDT = CADT.Object;
}

void UCBehave::TickComponent(const float DeltaTime, const enum ELevelTick TickType,
FActorComponentTickFunction* const ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (UNLIKELY(!ActionCur)) {
		SetComponentTickEnabled(false);
		PlanStart();
		return;
	}

	const EBDoRes Res = ActionCur->DoSelf(DeltaTime);
	if (LIKELY(Res == EBDoRes::CONTINUE)) return;
	
	if (Res == EBDoRes::STOP || Res == EBDoRes::ABORT) {
		CurStop();  // will plan next tick
	}
}

void UCBehave::BeginPlay() {
	Super::BeginPlay();

	for (const FName& N: ActionsToLoad) {
		UBBase* const B = LoadAction(N);
		if (UNLIKELY(!IsValid(B))) continue;
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

UBBase* UCBehave::NewAction(const TSubclassOf<UBBase>& Class) {
	UBBase* const B = NewObject<UBBase>(this, Class.Get());
	if (B) B->Register(this);
	return B;
}

UBBase* UCBehave::LoadAction(const FName Row) {
	UE_LOG(LogCBehave, Log, TEXT("%hs Row=%s"), __func__, *Row.ToString());
	
	if (UNLIKELY(!ActionsDT)) {
		UE_LOG(LogCBehave, Warning, TEXT("%hs datatable not loaded"), __func__);
		return nullptr;
	}

	FBAction* const ActDef = ActionsDT->FindRow<FBAction>(Row, "");
	if (UNLIKELY(!ActDef)) {
		UE_LOG(LogCBehave, Warning, TEXT("%hs could not find action id=%s"), __func__, *Row.ToString());
		return nullptr;
	}

	UBBase* const Action = NewAction(ActDef->Class);
	if (UNLIKELY(!Action)) {
		UE_LOG(LogCBehave, Warning, TEXT("%hs could not create action id=%s class=%s"),
			__func__, *Row.ToString(), *ActDef->Class.Get()->GetName());
		return nullptr;
	}
	
	Action->ID = Row;
	for (const FName& C: ActDef->Children) {
		UBBase* const Child = LoadAction(C);
		if (UNLIKELY(!Child)) continue; // load already warns
		Action->Children.Add(Child);
	}
	return Action;
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
	} else {
		// retry later
		FTimerHandle H;
		const UWorld* const World = GetWorld();
		if (World)
			World->GetTimerManager().SetTimer(H, this, &UCBehave::PlanStart, 1);
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
