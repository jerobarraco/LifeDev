// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "CAnimatorTrans.h"

void UCAnimatorTrans::DeInit() {
	TRoot = nullptr;
	Super::DeInit();
}

void UCAnimatorTrans::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	if (!IsValid(TRoot)) return;

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

void UCAnimatorTrans::BeginPlay() {
	Super::BeginPlay();
	// PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.Target = this;
	// PrimaryComponentTick.SetTickFunctionEnable(true);
	// RegisterComponentTickFunctions(true); will crash
	if (IsValid(TRoot) && IsAdditive) {
		TStart = TRoot->GetRelativeTransform();
	}
}
