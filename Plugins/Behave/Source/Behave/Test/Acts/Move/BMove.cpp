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
	if (Fish->Data.Tired > .7) return false;

	Target = FMath::RandPointInBox(FBox(FVector(0), FVector(100)));

	return true;
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

float UBMove::CostSelf_Implementation() const {
	if (!Fish) return 2;
	return Fish->Data.Tired + (Fish->GetActorLocation()-Target).Length()*.1;
	// return Super::CostSelf_Implementation();
}
