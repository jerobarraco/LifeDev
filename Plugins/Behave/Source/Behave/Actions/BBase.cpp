// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

#include "Behave/CBehave.h"

bool UBBase::Plan_Implementation() {
	CostPlanned = .5;
	return true;
}

EBDoRes UBBase::Do(const float DT) {
	if (UNLIKELY(State == EBState::STOPPED)) return EBDoRes::STOP;

	const EBDoRes R = DoSelf(DT);
	if (R != EBDoRes::STOP || !IsLooped) return R;
	// here is stopped and looping
	// restart
	SetState(EBState::STOPPED);
	SetState(EBState::STARTED);
	return EBDoRes::CONTINUE;
}

EBDoRes UBBase::DoSelf_Implementation(const float DT) {
	UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return EBDoRes::ABORT; // anomaly

	const EBDoRes R = C->DoSelf(DT);
	if (UNLIKELY(R == EBDoRes::ABORT)) return EBDoRes::ABORT; // bubble up (redundant with below, but this way i make sure that i don't refactor it out by mistake)

	// This base object is more abstract than anything else. so i will just return the child.
	// which is kind of the best thing, specially for classes that inherit from this
	return R;
}

void UBBase::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("%hs State=%s New=%s"),
		__func__, *UEnum::GetValueAsString(State), *UEnum::GetValueAsString(New));
	// const bool Stopping = New == EBState::STOPPED;
	// const bool DoLoop = Stopping && IsLooped; // try to loop only if we're stopping
	// State = DoLoop ? EBState::STARTED : New; // reset to start if we're looping. otherwise
	State = New;
	if (State == EBState::STOPPED)
		StopCurChild();

	OnState.Broadcast(this, State); // will broadcast stop of the parent after stop of children. which is good.
}

float UBBase::CostPlan_Implementation() const {
	// i could have returned the cost as outparam on Plan.
	// but i trust that people using this library is smart enough to read the documentation.
	// and also i plan to refactor this, so that instead of calling plan directly. it will perform an A*Search of sorts.
	// It might end up doing the same thing. as how can i give a cost without a plan???
	// i might change my mind later. or not. who knows. i'm a mystery.
	return CostPlanned;
}

UBBase* UBBase::GetCurChild() const {
	const int32 Num = Children.Num();
	if (UNLIKELY(CurChildI<0 || CurChildI>=Num)) return nullptr;
	return Children[CurChildI];
}

void UBBase::StopCurChild() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	SetCurChildSate(EBState::STOPPED);
	CurChildI = -1;
}

void UBBase::SetCurChildSate(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);

	// i could call GetCurChild here. but it's one more if. NOOooooooooouuuu!
	const int32 Num = Children.Num();
	if (UNLIKELY(CurChildI<0 || CurChildI>=Num)) return;
	Children[CurChildI]->SetState(New);
}

void UBBase::StartChild(const int32 I) {
	UE_LOG(LogTemp, Log, TEXT("%hs i=%i"), __func__, I);

	StopCurChild();

	if (UNLIKELY(I<0 || I>= Children.Num())) return;

	CurChildI = I;
	SetCurChildSate(EBState::STARTED);
}

void UBBase::DeInit_Implementation() {
	OnState.Clear();
	Behave = nullptr;
}

void UBBase::Init_Implementation(UCBehave* const B) {
	// done like this for these:
	// * allow to run actions upon registering (init)
	// * recurse into children
	// * keep a ref to ucbehave
	UE_LOG(LogTemp, Log, TEXT("UBBase:%hs O=%s"), __func__, *GetNameSafe(this));
	if (UNLIKELY(!B)) {
		UE_LOG(LogTemp, Warning, TEXT("UBBase:%hs Invalid Behave"), __func__);
		return;
	}
	// this is for ONLY on the bg thread. and we do need it on the game thread
	// also unsetting this is kind of hard. as this might be called but deinit not called if the user is doing something weird.
	// SetInternalFlags(EInternalObjectFlags::Async)
	Behave = B;
	Behave->TaskRegister(this);
	for (UBBase* const C: Children)
		if (LIKELY(C)) C->Init(Behave);
}
