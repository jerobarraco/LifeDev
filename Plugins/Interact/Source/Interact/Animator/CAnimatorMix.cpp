// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorMix.h"

void UCAnimatorMix::DeInit() {
	TRoot = nullptr;
	Mat = nullptr;
	Super::DeInit();
}

void UCAnimatorMix::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	if (IsValid(TRoot)) {
		FTransform TNew = TStart;
		if (IsAdditive) {
			TStart.BlendFromIdentityAndAccumulate(TNew, TEnd, (const ScalarRegister) Alpha);
			// TNew.Accumulate(TEnd, (const ScalarRegister) Alpha); // not what i want, does something different with the scale.
		}else {
			// Thanks Tim! this actually works very well!
			TNew.BlendWith(TEnd, Alpha);
		}
		TRoot->SetRelativeTransform(TNew);
	}

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
	// PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.Target = this;
	// PrimaryComponentTick.SetTickFunctionEnable(true);
	// RegisterComponentTickFunctions(true); will crash
	if (IsValid(TRoot)) {
		TStart = TRoot->GetRelativeTransform();
	}
}
