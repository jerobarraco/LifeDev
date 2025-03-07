// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CAnimatorMPC.h"

#include "Materials/MaterialParameterCollectionInstance.h"

void UCAnimatorMPC::DeInit() {
	MPC = nullptr;
	MPCI = nullptr;
	Super::DeInit();
}

void UCAnimatorMPC::Begin_Implementation() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	MPCI = World->GetParameterCollectionInstance(MPC);
	
	Super::Begin_Implementation(); // broadcasts
}

void UCAnimatorMPC::End_Implementation() {
	Super::End_Implementation();
	MPCI = nullptr;
}

void UCAnimatorMPC::Update_Implementation(const float Alpha) {
	Super::Update_Implementation(Alpha);

	if (UNLIKELY(!IsValid(MPCI))) {
		UE_LOG(LogTemp, Warning, TEXT("UCAnimatorMPC::%hs MPCI not found. Stop."), __func__);
		Deactivate();
		return;
	}
	
	if (!ParFName.IsNone()) {
		const float Val = FMath::LerpStable(ParFStart, ParFEnd, Alpha);
		MPCI->SetScalarParameterValue(ParFName, Val);
	}

	if (!ParVName.IsNone()) {
		const FLinearColor& Val =
			UseHSV ?
				FLinearColor::LerpUsingHSV(
					ParVStart, ParVEnd, Alpha) :
				FMath::LerpStable(ParVStart, ParVEnd, Alpha);
		MPCI->SetVectorParameterValue(ParVName, Val);
	}
}
