// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CAnimatorTrans.h"

void UCAnimatorTrans::DeInit() {
	TRoot = nullptr;
	Super::DeInit();
}

void UCAnimatorTrans::CopyTStart() {
	if (UNLIKELY(!TRoot)) return;
	TStart = TRoot->GetRelativeTransform();
}

void UCAnimatorTrans::Update_Implementation(const float Alpha) {
	Super::Update_Implementation(Alpha);

	if (!IsValid(TRoot)) return;
	// not unlikely since i use the animator mix, usually never the animator trans by itself.

	FTransform TNew = TStart;
	if (IsAdditive) {
		TStart.BlendFromIdentityAndAccumulate(
			TNew, TEnd, (const ScalarRegister) Alpha);
		// TNew.Accumulate(TEnd, (const ScalarRegister) Alpha); // not what i want, does something different with the scale.
	} else {
		// Thanks, Tim! this actually works very well!
		TNew.BlendWith(TEnd, Alpha);
	}
	TRoot->SetRelativeTransform(TNew, UseSweep);
}
