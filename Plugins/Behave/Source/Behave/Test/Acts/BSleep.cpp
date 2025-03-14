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

EBDoRes UBSleep::Do_Implementation(const float DT) {
	if (State == EBState::STOPPING) return EBDoRes::STOP;
	if (State == EBState::ABORTING) return EBDoRes::ABORT;
	if (!Fish) return EBDoRes::ABORT;
	if (Fish->Data.Tired<.2) return EBDoRes::STOP;
	Fish->Sleep(DT);
	return EBDoRes::CONTINUE;
}