// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorMix.h"

void UCAnimatorMix::DeInit() {
	Mat = nullptr;
	Super::DeInit();
}

void UCAnimatorMix::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	if (IsValid(Mat)) {
		if (!MatFName.IsNone()) {
			const float Val = FMath::LerpStable(MatFStart, MatFEnd, Alpha);
			Mat->SetScalarParameterValue(MatFName, Val);
		}

		if (!MatVName.IsNone()) {
			// more expensive but more cool
			const FLinearColor Val = FLinearColor::LerpUsingHSV(MatVStart, MatVEnd, Alpha);
			// const FLinearColor Val = FMath::Lerp(MatVStart, MatVEnd, Alpha);
			Mat->SetVectorParameterValue(MatVName, Val);
		}
	}
}

void UCAnimatorMix::BeginPlay() {
	Super::BeginPlay();
}
