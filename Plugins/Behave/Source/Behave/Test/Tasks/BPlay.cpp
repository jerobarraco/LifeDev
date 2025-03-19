// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BPlay.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"

UBPlay::UBPlay():Super() {
	ID = SID;
}

bool UBPlay::Plan_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Play::%hs"), __func__);
	const bool Ok = LIKELY(Fish) && Fish->Data.Tired < .3 && Fish->Data.Bored > .4;
	CostCur = Ok ? 1-Fish->Data.Bored: FLT_MAX;
	return Ok;
}

EBDoRes UBPlay::DoSelf_Implementation(const float DT) {
	if (UNLIKELY(!Fish)) return EBDoRes::ABORT;
	if (UNLIKELY(Fish->Data.Tired>.8)) return EBDoRes::STOP;
	if (UNLIKELY(Fish->Data.Bored<.2)) return EBDoRes::STOP;
	Fish->Play(DT);
	return EBDoRes::CONTINUE;
}