// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CAnimatorMix.h"

#include "Materials/MaterialInstanceDynamic.h"

void UCAnimatorMix::DeInit() {
	Mat = nullptr;
	Super::DeInit();
}

void UCAnimatorMix::Update_Implementation(const float Alpha) {
	Super::Update_Implementation(Alpha);

	if (!IsValid(Mat)) return;

	if (!MatFName.IsNone()) {
		const float Val = FMath::LerpStable(MatFStart, MatFEnd, Alpha);
		Mat->SetScalarParameterValue(MatFName, Val);
	}

	if (!MatVName.IsNone()) {
		// more expensive but more cool
		const FLinearColor& Val =
			UseHSV ?
				FLinearColor::LerpUsingHSV(
			MatVStart, MatVEnd, Alpha):
				FMath::Lerp(MatVStart, MatVEnd, Alpha);
		Mat->SetVectorParameterValue(MatVName, Val);
	}
}
