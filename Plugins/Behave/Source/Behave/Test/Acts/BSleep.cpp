// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSleep.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"

UBSleep::UBSleep():Super() {
	ID = SID;
}

bool UBSleep::Plan_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Sleep::%hs"), __func__);
	return Fish && Fish->Data.Tired > .7;
}

void UBSleep::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Sleep::%hs"), __func__);
	Super::SetState_Implementation(New);
	if (State == EBState::STARTED)
		TimeCur = 0; // reset
}

EBDoRes UBSleep::Do_Implementation(const float DT) {
	if (State == EBState::STOPPING) return EBDoRes::STOP;
	if (State == EBState::ABORTING) return EBDoRes::ABORT;
	if (Fish->Data.Tired<.2) return EBDoRes::STOP;
	Fish->Sleep(DT);
	return EBDoRes::CONTINUE;
}

void UBSleep::Register_Implementation(UCBehave* const B) {
	Super::Register_Implementation(B);
	if (UNLIKELY(!B)) return;
	Fish = Cast<ABFish>(B->GetOwner());
}
