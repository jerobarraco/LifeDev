// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BEat.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"

UBEat::UBEat():Super() {
	ID = SID;
}

bool UBEat::Plan_Implementation() {
	if (!Fish) return false;
	if (Fish->Data.Hungry > .7) return false;

	return true;
}

void UBEat::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Eat::%hs"), __func__);
	Super::SetState_Implementation(New);
}

EBDoRes UBEat::Do_Implementation(const float DT) {
	if (State == EBState::STOPPING) return EBDoRes::STOP;
	if (State == EBState::ABORTING) return EBDoRes::ABORT;
	if (!Fish) return EBDoRes::ABORT;
	
	Fish->Eat(DT);
	return EBDoRes::CONTINUE;
}
