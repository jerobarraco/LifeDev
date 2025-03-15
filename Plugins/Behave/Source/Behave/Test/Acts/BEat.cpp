// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BEat.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"

UBEat::UBEat():Super() {
	ID = SID;
}

bool UBEat::Plan_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Eat::%hs"), __func__);
	if (UNLIKELY(!Fish)) return false;
	if (Fish->Data.Hungry < .7) return false;

	return true;
}

void UBEat::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Eat::%hs"), __func__);
	Super::SetState_Implementation(New);
}

EBDoRes UBEat::DoSelf_Implementation(const float DT) {
	if (UNLIKELY(!Fish)) return EBDoRes::ABORT;
	if (UNLIKELY(Fish->Data.Hungry < .3)) return EBDoRes::STOP;
	
	Fish->Eat(DT);
	return EBDoRes::CONTINUE;
}
float UBEat::CostSelf_Implementation() const {
	return Super::CostSelf_Implementation();
}

