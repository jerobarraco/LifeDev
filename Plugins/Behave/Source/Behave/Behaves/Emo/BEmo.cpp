// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BEmo.h"

#include "Behave/Behaves/Bio/BBio.h"

UBEmo::UBEmo():Super() {
	Wants = {
		T_Anger, T_Sad, T_Fear, T_Shame, T_Joy, T_Bore
	};

	Values = {
		{T_Anger, 0},
		{T_Sad, 0},
		{T_Fear, 0},
		{T_Shame, 0},
		{T_Bore, 0}
	};
}

void UBEmo::Tick_Implementation(const float DT) {
	Super::Tick_Implementation(DT);
	const float Damp = EmoDamp * DT;
	bool AllZ = true;
	for (TTuple<FName, float>& KV: Values) {
		KV.Value = FMath::Clamp(KV.Value - Damp, 0, 1);
		if (KV.Key != T_Bore && KV.Value > .15)
			AllZ = false;
	}

	if (AllZ)
		Values[T_Bore] += Damp*5; // increase bore by emo_damp if no emotion is present
	// UE_LOG(LogTemp, Log, TEXT("%hs bore=%f"), __func__, Values[T_Bore]);
}

void UBEmo::ReactState_Implementation(const float DT, const FName& Token, const float Val) {
	Super::ReactState_Implementation(DT, Token, Val);
	if (UNLIKELY(Token == UBBio::T_Tired)) {
		const float Affect = Val < .2 ? FMath::Lerp(0.01, 0.001, Val *5) : 0;
		Mod(T_Anger, Affect*DT);
	} else if (UNLIKELY(Token == UBBio::T_Hungry)) {
		const float Affect = Val < .2 ? FMath::Lerp(0.01, 0.001, Val *5) : 0;
		Mod(T_Anger, Affect*DT);
	} else if (UNLIKELY(Token == T_Anger)) {
		const float Affect = Val > .95 ? .01:0;
		Mod(T_Sad, Affect*DT);
	} else if (UNLIKELY(Token == T_Sad)) {
		const float Affect = Val > .95 ? .01 : 0;
		// trigger a want to die?
	} 
}

