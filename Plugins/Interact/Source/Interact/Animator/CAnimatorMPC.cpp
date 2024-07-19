// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorMPC.h"

#include "Materials/MaterialParameterCollectionInstance.h"

void UCAnimatorMPC::DeInit() {
	MPC = nullptr;
	MPCI = nullptr;
	Super::DeInit();
}

void UCAnimatorMPC::Begin_Implementation() {
	const UWorld* const World = GetWorld();
	if (!World) return;

	MPCI = World->GetParameterCollectionInstance(MPC);
	
	Super::Begin_Implementation(); // broadcasts
}

void UCAnimatorMPC::End_Implementation() {
	Super::End_Implementation();
	MPCI = nullptr;
}

void UCAnimatorMPC::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	if (!IsValid(MPCI)) {
		UE_LOG(LogTemp, Warning, TEXT("UCAnimatorMPC::%hs MPC not found. Stop."), __func__);
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
