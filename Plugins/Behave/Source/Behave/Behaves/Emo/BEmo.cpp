// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BEmo.h"

void UBEmo::Tick_Implementation(const float DT) {
	Super::Tick_Implementation(DT);
	const float Damp = EmoDamp * DT;
	// TODO does this works?
	bool AllZ = true;
	for (TTuple<FName, float>& KV: Values) {
		KV.Value = FMath::Clamp(KV.Value - Damp, 0, 1);
		if (!FMath::IsNearlyZero(KV.Value))
			AllZ = false;
	}

	if (AllZ)
		Values[T_Bore] += Damp*2; // increase bore by emo_damp if no emotion is present
}

