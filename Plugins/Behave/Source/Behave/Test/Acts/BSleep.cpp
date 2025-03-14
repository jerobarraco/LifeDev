// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSleep.h"

UBSleep::UBSleep() {
	ID = SID;
}

bool UBSleep::Plan_Implementation() {
	return true; // TODO a way to read/write from globals
}

void UBSleep::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Sleep::%hs"), __func__);
	Super::SetState_Implementation(New);
	if (State == EBState::STARTED)
		TimeCur = 0; // reset
}

EBDoRes UBSleep::Do_Implementation(const float DT) {
	UE_LOG(LogTemp, Log, TEXT("Sleep::%hs"), __func__);
	if (TimeCur >= TimeSleep) return EBDoRes::STOP;

	TimeCur += TimeCur;
	return EBDoRes::CONTINUE;
}
