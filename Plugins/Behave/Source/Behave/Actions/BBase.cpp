// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

#include "Behave/CBehave.h"

bool UBBase::Plan_Implementation() {
	return true;
}

EBDoRes UBBase::Do_Implementation(const float DT) {
	UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return EBDoRes::ABORT; // anomaly

	const EBDoRes R = C->Do(DT);
	if (R == EBDoRes::ABORT) return EBDoRes::ABORT; // bubble up

	// This base object is more abstract than anything else. so i will just return the child.
	// which is kind of the best thing, specially for classes that inherit from this
	return R;
}

void UBBase::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("%hs State=%s New=%s"),
		__func__, *UEnum::GetValueAsString(State), *UEnum::GetValueAsString(New));
	State = New;

	if (State == EBState::STOPPED) StopCurChild();
	
	OnState.Broadcast(this, State);
}

float UBBase::CostPlan_Implementation() const {
	// i could have returned the cost as outparam on Plan.
	// but i trust that people using this library is smart enough to read the documentation.
	// and also i plan to refactor this, so that instead of calling plan directly. it will perform an A*Search of sorts.
	// It might end up doing the same thing. as how can i give a cost without a plan???
	// i might change my mind later. or not. who knows. i'm a mystery.
	return CostPlanned;
}

float UBBase::CostSelf_Implementation() const { return 0; }

UBBase* UBBase::GetCurChild() const {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);

	const int32 Num = Children.Num();
	if (UNLIKELY(CurChildI<0 || CurChildI>=Num)) return nullptr;
	return Children[CurChildI];
}

void UBBase::StopCurChild() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	SetCurChildSate(EBState::STOPPED);
}

void UBBase::SetCurChildSate(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);

	// i could call GetCurChild here. but it's one more if. NOOooooooooouuuu!
	const int32 Num = Children.Num();
	if (UNLIKELY(CurChildI<0 || CurChildI>=Num)) return;
	Children[CurChildI]->SetState(New);
}

void UBBase::StartChild(const int32 I) {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);

	StopCurChild();

	if (I<0 || I>= Children.Num()) {
		CurChildI = -1;
		return;
	}

	CurChildI = I;
	SetCurChildSate(EBState::STARTED);
}

void UBBase::Register(UCBehave* const B) {
	if (!B) {
		UE_LOG(LogTemp, Warning, TEXT("UBBase:%hs cant get the behave outer"), __func__);
		return;
	}

	Behave = B;
	Behave->Register(this);
	for (UBBase* const C: Children) {
		if (!C) return;
		C->Register(Behave);
	}
}
