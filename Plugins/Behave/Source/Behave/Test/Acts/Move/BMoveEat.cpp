// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMoveEat.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"

UBMoveEat::UBMoveEat():Super() {
	ID = SID;
}

bool UBMoveEat::Plan_Implementation() {
	if (!Fish) return false;
	if (Fish->Data.Tired > .7) return false;

	Target = FMath::RandPointInBox(FBox(FVector(0), FVector(100)));

	return true;
}

void UBMoveEat::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("MoveEat::%hs"), __func__);
	Super::SetState_Implementation(New);
}

EBDoRes UBMoveEat::Do_Implementation(const float DT) {
	if (State == EBState::STOPPING) return EBDoRes::STOP;
	if (State == EBState::ABORTING) return EBDoRes::ABORT;
	if (!Fish) return EBDoRes::ABORT;
	
	const FVector& Current = Fish->GetActorLocation();
	const float Dist = FVector::DistSquared(Current, Target);
	if (Dist < 1) return EBDoRes::STOP;

	Fish->MoveTo(Target, DT);
	return EBDoRes::CONTINUE;
}
