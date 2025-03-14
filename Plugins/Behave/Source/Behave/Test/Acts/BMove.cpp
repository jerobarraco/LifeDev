// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMove.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"

UBMove::UBMove():Super() {
	ID = SID;
}

bool UBMove::Plan_Implementation() {
	if (!Fish) return false;
	
	// TODO get target. calculate cost.
	return true; // TODO a way to read/write from globals
}

void UBMove::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Move::%hs"), __func__);
	Super::SetState_Implementation(New);
}

EBDoRes UBMove::Do_Implementation(const float DT) {
	if (State == EBState::STOPPING) return EBDoRes::STOP;
	if (State == EBState::ABORTING) return EBDoRes::ABORT;
	if (!Fish) return EBDoRes::ABORT;
	
	const FVector& Current = Fish->GetActorLocation();
	const float Dist = FVector::DistSquared(Current, Target);
	if (Dist < 1) return EBDoRes::STOP;

	Fish->MoveTo(Target, DT);
	return EBDoRes::CONTINUE;
}

void UBMove::Register_Implementation(UCBehave* const B) {
	Super::Register_Implementation(B);
	if (UNLIKELY(!B)) return;

	Fish = Cast<ABFish>(B->GetOwner());
}
