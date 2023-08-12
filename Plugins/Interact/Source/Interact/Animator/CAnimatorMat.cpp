// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "CAnimatorMat.h"

void UCAnimatorMat::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);
	if (!IsValid(Mat)) return;

	if (!ParamFName.IsNone()) {
		const float Val = FMath::LerpStable(FStart, FEnd, Alpha);
		Mat->SetScalarParameterValue(ParamFName, Val);
	}

	if (!ParamVName.IsNone()) {
		// const FVector Val = FMath::LerpStable(VStart, VEnd, Alpha);
		const FLinearColor Val = FLinearColor::LerpUsingHSV(VStart, VEnd, Alpha);
		Mat->SetVectorParameterValue(ParamVName, Val);
	}
}

void UCAnimatorMat::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Mat = nullptr;
}
