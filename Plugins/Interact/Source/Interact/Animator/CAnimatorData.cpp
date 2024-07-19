// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorData.h"

void UCAnimatorData::DeInit() {
	Prim = nullptr;
	Super::DeInit();
}

void UCAnimatorData::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);
	
	if (!IsValid(Prim)) return;
	if (DataFIndex >= 0) {
		const float Val = FMath::LerpStable(DataFStart, DataFEnd, Alpha);
		Prim->SetCustomPrimitiveDataFloat(DataFIndex, Val);
	}

	if (DataVIndex >= 0) {
		const FLinearColor& Val = UseHSV ?
				FLinearColor::LerpUsingHSV(DataVStart, DataVEnd, Alpha) :
				FMath::LerpStable(DataVStart, DataVEnd, Alpha);
		Prim->SetCustomPrimitiveDataVector4(DataVIndex, Val);
		// this is the functions to call, the ones that use an FName are for dynamic materials
	}
}
