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
	if (Fish->Data.Tired > .7) return false;

	Target = FMath::RandPointInBox(FBox(FVector(0), FVector(100)));

	return true;
}

void UBEat::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Move::%hs"), __func__);
	Super::SetState_Implementation(New);
}

EBDoRes UBEat::Do_Implementation(const float DT) {
	if (State == EBState::STOPPING) return EBDoRes::STOP;
	if (State == EBState::ABORTING) return EBDoRes::ABORT;
	if (!Fish) return EBDoRes::ABORT;
	
	const FVector& Current = Fish->GetActorLocation();
	const float Dist = FVector::DistSquared(Current, Target);
	if (Dist < 1) return EBDoRes::STOP;

	Fish->MoveTo(Target, DT);
	return EBDoRes::CONTINUE;
}

void UBEat::Register_Implementation(UCBehave* const B) {
	// TODo i can move this to a base one
	Super::Register_Implementation(B);
	if (UNLIKELY(!B)) return;

	Fish = Cast<ABFish>(B->GetOwner());
}
