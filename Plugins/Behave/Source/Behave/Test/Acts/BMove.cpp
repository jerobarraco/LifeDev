// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMove.h"

UBMove::UBMove():Super() {
	ID = SID;
}

bool UBMove::Plan_Implementation() {
	// TODO get target. calculate cost.
	return true; // TODO a way to read/write from globals
}

void UBMove::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Move::%hs"), __func__);
	Super::SetState_Implementation(New);
}

EBDoRes UBMove::Do_Implementation(const float DT) {
	return EBDoRes::CONTINUE;
}
