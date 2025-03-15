// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BMove.h"

#include "Behave/CBehave.h"
#include "Behave/Test/BFish.h"

UBMove::UBMove():Super() {
	ID = SID;
}

bool UBMove::Plan_Implementation() {
	const bool Ok = LIKELY(Fish) && (Fish->Data.Tired < .7);
	Target = FMath::RandPointInBox(FBox(FVector(0), FVector(50)));
	const float Dist = FVector::DistSquared(Fish->GetActorLocation(), Target);
	CostPlanned = Fish->Data.Tired + (Dist*.01);

	return Ok;
}

void UBMove::SetState_Implementation(const EBState New) {
	UE_LOG(LogTemp, Log, TEXT("Move::%hs"), __func__);
	Super::SetState_Implementation(New);
}

EBDoRes UBMove::DoSelf_Implementation(const float DT) {
	if (UNLIKELY(!Fish)) return EBDoRes::ABORT;
	
	const FVector& Current = Fish->GetActorLocation();
	const float Dist = FVector::DistSquared(Current, Target);
	if (UNLIKELY(Dist < 1)) return EBDoRes::STOP;

	Fish->MoveTo(Target, DT);
	return EBDoRes::CONTINUE;
}
