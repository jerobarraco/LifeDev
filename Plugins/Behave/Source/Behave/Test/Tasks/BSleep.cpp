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
	const bool Ok = LIKELY(Fish) && Fish->Data.Tired > .7;
	CostCur = Ok ? 1-Fish->Data.Tired: FLT_MAX;
	return Ok;
}

EBDoRes UBSleep::DoSelf_Implementation(const float DT) {
	if (UNLIKELY(!Fish)) return EBDoRes::ABORT;
	if (UNLIKELY(Fish->Data.Tired<.2)) return EBDoRes::STOP;

	Fish->Sleep(DT);
	return EBDoRes::CONTINUE;
}